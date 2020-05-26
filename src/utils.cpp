//! \file
/*
**  Copyright (c) 2020 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

//C++
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
//Used in GetTimeMs64
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif


//Triton
#include <triton/api.hpp>
#include <triton/x86Specifications.hpp>

//IDA
#include <idp.hpp>
#include <loader.hpp>
#include <dbg.hpp>
#include <name.hpp>
#include <bytes.hpp>

//Ponce
#include "utils.hpp"
#include "globals.hpp"
#include "context.hpp"
#include "blacklist.hpp"
#include "callbacks.hpp"





/*This functions gets a string and return the triton register assign or nullptr
This is using the triton current architecture so it is more generic.*/
const triton::arch::register_e str_to_register(const qstring& register_name)
{
    for (const auto& [key, value] : api.getAllRegisters())
        if (stricmp(value.getName().c_str(), register_name.c_str()) == 0)
            return key;

    return triton::arch::register_e::ID_REG_INVALID;
}


/*This function is a helper to find a function having its name.
It is likely IDA SDK has another API to do this but I can't find it.
Source: http://www.openrce.org/reference_library/files/ida/idapw.pdf */
ea_t find_function(char const* function_name)
{
    // get_func_qty() returns the number of functions in file(s) loaded.
    for (unsigned int f = 0; f < get_func_qty(); f++) {
        // getn_func() returns a func_t struct for the function number supplied
        func_t* curFunc = getn_func(f);
        qstring funcName;
        ssize_t size_read = 0;
        // get_func_name2 gets the name of a function and stored it in funcName
        size_read = get_func_name(&funcName, curFunc->start_ea);
        if (size_read > 0) { // if found
            if (strcmp(funcName.c_str(), function_name) == 0) {
                return curFunc->start_ea;
            }
            //We need to ignore our prefix when the function is tainted
            //If the function name starts with our prefix, fix for #51
            if (strstr(funcName.c_str(), RENAME_TAINTED_FUNCTIONS_PREFIX) == funcName.c_str() && funcName.size() > RENAME_TAINTED_FUNCTIONS_PATTERN_LEN) {
                //Then we ignore the prefix and compare the rest of the function name
                if (strcmp(funcName.c_str() + RENAME_TAINTED_FUNCTIONS_PATTERN_LEN, function_name) == 0) {
                    return curFunc->start_ea;
                }
            }
        }
    }
    return -1;
}

//This function return the real value of the argument.
ea_t get_args(int argument_number, bool skip_ret)
{
#if !defined(__EA64__)
    ea_t memprogram = get_args_pointer(argument_number, skip_ret);
    //We first get the pointer and then we dereference it
    ea_t value = 0;
    value = read_regSize_from_ida(memprogram);
    return value;

#else
    int skip_ret_index = skip_ret ? 1 : 0;
    //Not converted to IDA we should use get_reg_val
#ifdef __NT__ // note the underscore: without it, it's not msdn official!
    // On Windows - function parameters are passed in using RCX, RDX, R8, R9 for ints / ptrs and xmm0 - 3 for float types.
    switch (argument_number)
    {
    case 0: return IDA_getCurrentRegisterValue(api.registers.x86_rcx).convert_to<ea_t>();
    case 1: return IDA_getCurrentRegisterValue(api.registers.x86_rdx).convert_to<ea_t>();
    case 2: return IDA_getCurrentRegisterValue(api.registers.x86_r8).convert_to<ea_t>();
    case 3: return IDA_getCurrentRegisterValue(api.registers.x86_r9).convert_to<ea_t>();
    default:
        ea_t esp = (ea_t)IDA_getCurrentRegisterValue(api.registers.x86_rsp).convert_to<ea_t>();
        ea_t arg = esp + (argument_number - 4 + skip_ret_index) * 8;
        return get_qword(arg);
    }
#elif __LINUX__ || __MAC__ // IDA macros https://www.hex-rays.com/products/ida/support/sdkdoc/pro_8h.html
    //On Linux - parameters are passed in RDI, RSI, RDX, RCX, R8, R9 for ints / ptrs and xmm0 - 7 for float types.
    switch (argument_number)
    {
    case 0: return IDA_getCurrentRegisterValue(api.registers.x86_rdi).convert_to<ea_t>();
    case 1: return IDA_getCurrentRegisterValue(api.registers.x86_rsi).convert_to<ea_t>();
    case 2: return IDA_getCurrentRegisterValue(api.registers.x86_rdx).convert_to<ea_t>();
    case 3: return IDA_getCurrentRegisterValue(api.registers.x86_rcx).convert_to<ea_t>();
    case 4: return IDA_getCurrentRegisterValue(api.registers.x86_r8).convert_to<ea_t>();
    case 5: return IDA_getCurrentRegisterValue(api.registers.x86_r9).convert_to<ea_t>();
    default:
        ea_t esp = (ea_t)IDA_getCurrentRegisterValue(api.registers.x86_rsp);
        ea_t arg = esp + (argument_number - 6 + skip_ret_index) * 8;
        return get_qword(arg);
    }
#endif
#endif
}

// Return the argument at the "argument_number" position. It is independant of the architecture and the OS.
// We suppossed the function is using the default call convention, stdcall or cdelc in x86, no fastcall and fastcall in x64
ea_t get_args_pointer(int argument_number, bool skip_ret)
{
    int skip_ret_index = skip_ret ? 1 : 0;
#if !defined(__EA64__)
    regval_t esp_value;
    invalidate_dbg_state(DBGINV_REGS);
    get_reg_val("esp", &esp_value);
    ea_t arg = (ea_t)esp_value.ival + (argument_number + skip_ret_index) * 4;
    return arg;
#else
    //Not converted to IDA we should use get_reg_val
#ifdef __NT__ // note the underscore: without it, it's not msdn official!
    // On Windows - function parameters are passed in using RCX, RDX, R8, R9 for ints / ptrs and xmm0 - 3 for float types.
    switch (argument_number)
    {
    case 0:
    case 1:
    case 2:
    case 3: error("[!] In Windows 64 bits you can't get a pointer to the four first\n arguments since they are registers");
    default:
        ea_t esp = (ea_t)IDA_getCurrentRegisterValue(api.registers.x86_rsp).convert_to<ea_t>();
        ea_t arg = esp + (argument_number - 4 + skip_ret_index) * 8;
        return arg;
    }
#elif __LINUX__ || __MAC__ // IDA macros https://www.hex-rays.com/products/ida/support/sdkdoc/pro_8h.html
    //On Linux - parameters are passed in RDI, RSI, RDX, RCX, R8, R9 for ints / ptrs and xmm0 - 7 for float types.
    switch (argument_number)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:error("[!] In Linux/OsX 64 bits you can't get a pointer to the five first\n arguments since they are registers");
    default:
        ea_t esp = (ea_t)IDA_getCurrentRegisterValue(api.registers.x86_rsp);
        ea_t arg = esp + (argument_number - 6 + skip_ret_index) * 8;
        return arg;
    }
#endif
#endif
}

//Use templates??
short read_unicode_char_from_ida(ea_t address)
{
    short value;
    //This is the way to force IDA to read the value from the debugger
    //More info here: https://www.hex-rays.com/products/ida/support/sdkdoc/dbg_8hpp.html#ac67a564945a2c1721691aa2f657a908c
    invalidate_dbgmem_contents(address, sizeof(value));
    ssize_t bytes_read = get_bytes(&value, sizeof(value), address, GMB_READALL, NULL);
    if (bytes_read == 0 || bytes_read == -1) {
        msg("[!] Error reading memory from " MEM_FORMAT "\n", address);
    }
    return value;
}

//Use templates??
char read_char_from_ida(ea_t address)
{
    char value;
    //This is the way to force IDA to read the value from the debugger
    //More info here: https://www.hex-rays.com/products/ida/support/sdkdoc/dbg_8hpp.html#ac67a564945a2c1721691aa2f657a908c
    invalidate_dbgmem_contents(address, sizeof(value));
    ssize_t bytes_read = get_bytes(&value, sizeof(value), address, GMB_READALL, NULL);
    if (bytes_read == 0 || bytes_read == -1) {
        msg("[!] Error reading memory from " MEM_FORMAT "\n", address);
    }
    return value;
}

ea_t read_regSize_from_ida(ea_t address)
{
    ea_t value;
    //This is the way to force IDA to read the value from the debugger
    //More info here: https://www.hex-rays.com/products/ida/support/sdkdoc/dbg_8hpp.html#ac67a564945a2c1721691aa2f657a908c
    invalidate_dbgmem_contents(address, sizeof(value));
    ssize_t bytes_read = get_bytes(&value, sizeof(value), address, GMB_READALL, NULL);
    if (bytes_read == 0 || bytes_read == -1) {
        msg("[!] Error reading memory from " MEM_FORMAT "\n", address);
    }

    return value;
}

/*This function renames a tainted function with the prefix RENAME_TAINTED_FUNCTIONS_PATTERN, by default "T%03d_"*/
void rename_tainted_function(ea_t address)
{
    qstring func_name;
    ssize_t size = 0x0;
    //First we get the current function name
    size = get_func_name(&func_name, address);

    if (size > 0) {
        //If the function isn't already renamed
        if (strstr(func_name.c_str(), RENAME_TAINTED_FUNCTIONS_PREFIX) != func_name.c_str()) {
            char new_func_name[MAXSTR];
            //This is a bit tricky, the prefix contains the format string, so if the user modified it and removes the format string isn't going to work
            qsnprintf(new_func_name, sizeof(new_func_name), RENAME_TAINTED_FUNCTIONS_PATTERN"%s", ponce_runtime_status.tainted_functions_index, func_name.c_str());
            //We need the start of the function we can have that info with our function find_function
            set_name(find_function(func_name.c_str()), new_func_name);
            if (cmdOptions.showDebugInfo)
                msg("[+] Renaming function %s -> %s\n", func_name.c_str(), new_func_name);
            ponce_runtime_status.tainted_functions_index += 1;
        }
    }
}

void add_symbolic_expressions(triton::arch::Instruction* tritonInst, ea_t address)
{
    std::ostringstream oss;
    for (const auto& expr : tritonInst->symbolicExpressions) {       
        oss << expr << "\n";
    }

    ponce_set_cmt(address, oss.str().c_str(), false);
}

std::string notification_code_to_string(int notification_code)
{
    switch (notification_code)
    {
    case 0:
        return std::string("dbg_null");
    case 1:
        return std::string("dbg_process_start");
    case 2:
        return std::string("dbg_process_exit");
    case 3:
        return std::string("dbg_process_attach");
    case 4:
        return std::string("dbg_process_detach");
    case 5:
        return std::string("dbg_thread_start");
    case 6:
        return std::string("dbg_thread_exit");
    case 7:
        return std::string("dbg_library_load");
    case 8:
        return std::string("dbg_library_unload");
    case 9:
        return std::string("dbg_information");
    case 10:
        return std::string("dbg_exception");
    case 11:
        return std::string("dbg_suspend_process");
    case 12:
        return std::string("dbg_bpt");
    case 13:
        return std::string("dbg_trace");
    case 14:
        return std::string("dbg_request_error");
    case 15:
        return std::string("dbg_step_into");
    case 16:
        return std::string("dbg_step_over");
    case 17:
        return std::string("dbg_run_to");
    case 18:
        return std::string("dbg_step_until_ret");
    case 19:
        return std::string("dbg_bpt_changed");
    case 20:
        return std::string("dbg_last");
    default:
        return std::string("Not defined");
    }
}

/*This function loads the options from the config file.
It returns true if it reads the config false, if there is any error.*/
bool load_options(struct cmdOptionStruct* cmdOptions)
{
    std::ifstream config_file;
    config_file.open("Ponce.cfg", std::ios::in | std::ios::binary);
    if (!config_file.is_open())
    {
        msg("[i] Config file %s not found\n", "Ponce.cfg");
        return false;
    }
    auto begin = config_file.tellg();
    config_file.seekg(0, std::ios::end);
    auto end = config_file.tellg();
    config_file.seekg(0, std::ios::beg);
    if ((end - begin) != sizeof(struct cmdOptionStruct))
        return false;
    config_file.read((char*)cmdOptions, sizeof(struct cmdOptionStruct));
    config_file.close();

    //Check if we need to reload the custom blacklisted functions
    if (cmdOptions->blacklist_path[0] != '\0') {
        //Means that the user set a path for custom blacklisted functions
        if (blacklkistedUserFunctions != NULL) {
            //Check if we had a previous custom blacklist and if so we delete it
            blacklkistedUserFunctions->clear();
            delete blacklkistedUserFunctions;
            blacklkistedUserFunctions = NULL;
        }
        readBlacklistfile(cmdOptions->blacklist_path);
    }

    return true;
}

/*This function loads the options from the config file.
It returns true if it reads the config false, if there is any error.*/
bool save_options(struct cmdOptionStruct* cmdOptions)
{
    std::ofstream config_file;
    config_file.open("Ponce.cfg", std::ios::out | std::ios::binary);
    if (!config_file.is_open())
    {
        msg("[!] Error opening config file %s\n", "Ponce.cfg");
        return false;
    }
    config_file.write((char*)cmdOptions, sizeof(struct cmdOptionStruct));
    config_file.close();
    return true;
}




/*Ask to the user if he really want to execute native code even if he has a snapshot.
Returns true if the user say yes.*/
bool ask_for_execute_native()
{
    //Is there any snapshot?
    if (!snapshot.exists())
        return true;
    //If so we should say to the user that he cannot execute native code and expect the snapshot to work
    int answer = ask_yn(1, "[?] If you execute native code (without tracing) Ponce cannot trace all the memory modifications so the execution snapshot will be deleted. Do you still want to do it? (Y/n):");
    if (answer == 1) //Yes
        return true;
    else // No or Cancel
        return false;
}

/*This function deletes the prefixes and sufixes that IDA adds*/
qstring clean_function_name(qstring name) {
    if (name.substr(0, 7) == "__imp__")
        return clean_function_name(name.substr(7));
    else if (name.substr(0, 4) == "imp_")
        return clean_function_name(name.substr(4));
    else if (name.substr(0, 3) == "cs:" || name.substr(0, 3) == "ds:")
        return clean_function_name(name.substr(3));
    else if (name.substr(0, 2) == "j_")
        return clean_function_name(name.substr(2));
    else if (name.substr(0, 1) == "_" || name.substr(0, 1) == "@" || name.substr(0, 1) == "?")
        return clean_function_name(name.substr(1));
    else if (name.find('@', 0) != -1)
        return clean_function_name(name.substr(0, name.find('@', 0)));
    else if (name.at(name.length() - 2) == '_' && isdigit(name.at(name.length() - 1))) //name_1
        return clean_function_name(name.substr(0, name.length() - 2));
    return name;
}

qstring get_callee_name(ea_t address) {
    qstring name;
    char buf[100] = { 0 };
    static const char* nname = "$ vmm functions";
    netnode n(nname);
    auto fun = n.altval(address) - 1;
    if (fun == -1) {
        qstring buf_op;
        if (is_code(get_flags(address)))
            print_operand(&buf_op, address, 0);
        qstring buf_tag;
        tag_remove(&buf_tag, buf_op);
        name = clean_function_name(buf_tag);
    }
    else {
        get_ea_name(&name, fun); // 00C5101A call    edi ; __imp__malloc style

        if (name.empty()) {
            qstring buf_op;
            if (is_code(get_flags(address)))
                print_operand(&buf_op, address, 0);
            qstring buf_tag;
            tag_remove(&buf_tag, buf_op);
            name = clean_function_name(buf_tag);
        }
        else {
            name = clean_function_name(name);
        }
    }
    return name;
}

std::uint64_t GetTimeMs64(void)
{
#ifdef _WIN32
    /* Windows */
    FILETIME ft;
    LARGE_INTEGER li;

    /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
    * to a LARGE_INTEGER structure. */
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    std::uint64_t ret = li.QuadPart;
    ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
    ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

    return ret;
#else
    /* Linux */
    struct timeval tv;

    gettimeofday(&tv, NULL);

    triton::uint64 ret = tv.tv_usec;
    /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
    ret /= 1000;

    /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
    ret += (tv.tv_sec * 1000);

    return ret;
#endif
}

/* Gets current instruction. Only possible if */
ea_t current_instruction()
{
    if (!is_debugger_on()) {
        return 0;
    }
    ea_t xip = 0;
    if (!get_ip_val(&xip)) {
        msg("[-] Could not get the XIP value. This should never happen\n");
        return 0;
    }
    return xip;
}

/* This function deletes all the comments and colour made by Ponce Plugin everytime that the Ponce
engine is restarted with another run. We do this to prevent polluting the IDA UI*/
void delete_ponce_comments() {
    unsigned int count_comments = 0;
    unsigned int count_colors = 0;
    ea_t snapshot_address = 0;
    for (auto& [address, insinfo]: ponce_comments) {      
        if (!insinfo.comment.empty()) { //comment
            set_cmt(address, "", false);
            count_comments++;
        }
        if (!insinfo.snapshot_comment.empty()) { //extra comment
            if (snapshot.exists())
                snapshot_address = address;
            set_cmt(address, "", false);
        }
        if (insinfo.color != DEFCOLOR) { //color
            del_item_color(address);
            count_colors++;
        }
    }
    ponce_comments.clear();
    msg("[+] Deleted %u comments and %u colored addresses\n", count_comments, count_colors);

    // If snapshot exists lets put it back
    if (snapshot_address) {
        ponce_set_cmt(snapshot_address, "Snapshot taken here", false, true);
    }
}

void ponce_set_item_color(ea_t ea, bgcolor_t color) {
    //if it is a new color we add it to ponce_comments
    if (ponce_comments.count(ea) > 0) {
        ponce_comments[ea].color = color;
    }
    else {
        struct instruction_info insinfo;
        insinfo.color = color;
        ponce_comments[ea] = insinfo;
    }
    set_item_color(ea, color);
}

/* Wrapper to keep track of added comments so we can delete them after*/
bool ponce_set_cmt(ea_t ea, const char* comm, bool rptble, bool snapshot) {
    qstring buf;
    qstring new_comment;
    if (get_cmt(&buf, ea, rptble) != -1) {
        auto first_space = strchr(buf.c_str(), ' ');
        // there is a previous comment. Let's try to get the hit count
        if (first_space){
            try {
                auto n_hit = std::stoi(std::string(buf.c_str(), first_space - buf.c_str()));
                new_comment.sprnt("%d hits. %s", ++n_hit, comm);
                //return set_cmt(ea, new_comment.c_str(), rptble);
            }
            catch (...) {}
        }
        if(new_comment.empty()){
            new_comment.sprnt("%d hits. %s", 2, comm);
            //return set_cmt(ea, new_comment.c_str(), rptble);     
        }
    }
    else { //its a new comment
        new_comment = comm;
    }

    auto new_line_pos = new_comment.find('\n');
    /* Lets only get the text about Symbolic/Taint instruction not the memory or
        registers involved since thats not relevant info for the pseudocode*/
    std::string pseudocode_comment;
    if (new_line_pos != std::string::npos) 
        pseudocode_comment = std::string(new_comment.c_str(), new_line_pos);
    else
        pseudocode_comment = std::string(new_comment.c_str());

    //if it is a new comment we add it to ponce_comments
    if (ponce_comments.count(ea) > 0) {
        if (snapshot)
            ponce_comments[ea].snapshot_comment = pseudocode_comment;
        else
            ponce_comments[ea].comment = pseudocode_comment;
    }
    else  {
        struct instruction_info insinfo;
        if (snapshot)
            insinfo.snapshot_comment = pseudocode_comment;
        else
            insinfo.comment = pseudocode_comment;

        ponce_comments[ea] = insinfo;
    }
    return set_cmt(ea, new_comment.c_str(), rptble);
}

/*This function gets the tainted operands for an instruction and add a comment to that instruction with this info*/
void comment_controlled_operands(triton::arch::Instruction* tritonInst, ea_t pc)
{
    std::stringstream comment;
    std::stringstream regs_controlled;
    std::stringstream mems_controlled;
    std::stringstream coso;

    if ((cmdOptions.use_symbolic_engine && !tritonInst->isSymbolized()) || (cmdOptions.use_tainting_engine && !tritonInst->isTainted())) {
        return;
    }

    if (cmdOptions.use_tainting_engine)
        comment << "Tainted instruction";
    else
        comment << "Symbolic instruction";

    /*Here we check all the registers and memory read to know which are tainted*/
    auto regs = tritonInst->getReadRegisters();
    for (const auto& [reg, expr] : tritonInst->getReadRegisters()) {
        if ((cmdOptions.use_tainting_engine && api.isRegisterTainted(reg)) ||
            (cmdOptions.use_symbolic_engine && api.isRegisterSymbolized(reg))) {
            regs_controlled << std::uppercase << reg.getName() << " ";
        }
    }
    if (regs_controlled.str().size() > 0) {
        comment << "\nRegister: " << regs_controlled.str();
    }

    for (const auto& [mem, expr] : tritonInst->getLoadAccess()) {
        if ((cmdOptions.use_tainting_engine && api.isMemoryTainted(mem)) ||
            (cmdOptions.use_symbolic_engine && api.isMemorySymbolized(mem)))
            mems_controlled << "0x" << std::hex << mem.getAddress() << " ";
    }

    if (mems_controlled.str().size() > 0) {
        comment << "\nMemory: " << mems_controlled.str();
    }

    //We set the comment
    if (comment.str().size() > 0) {
        ponce_set_cmt(pc, comment.str().c_str(), false);
    }
}

//! \file
/*
**  Copyright (c) 2016 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

#ifdef BUILD_HEXRAYS_SUPPORT
#include "ponce_hexrays.hpp"
#include "globals.hpp"

#include <hexrays.hpp>

/* Returns the pseudocode line corresponding to an address or -1 if fail*/
int get_compile_coord_by_ea(cfunc_t* cfunc, ea_t addr) {
    int y = -1;
#if IDA_SDK_VERSION >= 720
    if (citem_t* item = cfunc->body.find_closest_addr(addr)) {
        if (cfunc->find_item_coords(item, nullptr, &y)) {
            //found
        }
        else
            y = -1;
    }
#else
    std::map<ea_t, int> lnmap;
    int i = 0;
    y = -1;

    for (const auto& line : cfunc->get_pseudocode()) {
        auto pitem = ctree_item_t();
        auto ret = cfunc->get_line_item(line.line.c_str(), 0, true, nullptr, &pitem, nullptr);
        if (ret && pitem.it) {
            lnmap[pitem.it->ea] = i;
        }
        i++;
    }
    ea_t closest_ea = BADADDR;
    for (const auto& [ea, line] : lnmap) {
        if (closest_ea == BADADDR || abs(int(closest_ea - addr)) > abs(int(ea - addr))) {
            closest_ea = ea;
            y = lnmap[ea];
        }
    }
#endif
    return y;

}

#if IDA_SDK_VERSION == 700
int idaapi ponce_hexrays_callback(void*, hexrays_event_t event, va_list va)
#else
ssize_t idaapi ponce_hexrays_callback(void*, hexrays_event_t event, va_list va)
#endif
{
    int y = -1;
    
    switch (event)
    {
    case hxe_func_printed:
    {
        cfunc_t* cfunc = va_arg(va, cfunc_t*);
        ctree_maturity_t mat = va_argi(va, ctree_maturity_t);
#if IDA_SDK_VERSION == 700
        func_t* func = get_func(cfunc->entry_ea);
#else
        func_t* func = cfunc->mba->get_curfunc();
#endif
        std::list<int> already_commented_lines;
        for (const auto& [address, insinfo] : ponce_comments) {
            if (func_contains(func, address)) {
                y = get_compile_coord_by_ea(cfunc, address);
                if (y == -1)
                    continue;
                if (!insinfo.comment.empty()) { //comment
                    if (std::find(already_commented_lines.begin(), already_commented_lines.end(), y) != already_commented_lines.end()) {
                        // We have already commented this line. Don't do it again
                        continue;
                    }
                    cfunc->sv[y].line.cat_sprnt("\t\t\t" COLSTR("// %s", SCOLOR_NUMBER), insinfo.comment.c_str());
                    already_commented_lines.push_back(y);
                }
                if (!insinfo.snapshot_comment.empty()) { //extra comment
                    if (std::find(already_commented_lines.begin(), already_commented_lines.end(), y) != already_commented_lines.end()) {
                        // We have already commented this line. Don't do it again
                        continue;
                    }
                    cfunc->sv[y].line += "\t\t\t/*";
                    cfunc->sv[y].line += insinfo.comment.c_str();
                    cfunc->sv[y].line += "*/";
                    already_commented_lines.push_back(y);
                }
                if (insinfo.color != DEFCOLOR && insinfo.color != cmdOptions.color_executed_instruction) { //color
                    cfunc->sv[y].bgcolor = insinfo.color;
                }
            }
        }       
        break;
    }
    default:
        break;
    }

    return 0;
}
#endif
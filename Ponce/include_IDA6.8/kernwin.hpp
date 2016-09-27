/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

/*! \file kernwin.hpp

  \brief Defines the interface between the kernel and the UI.

  It contains:
          - the UI dispatcher notification codes (::ui_notification_t)
          - convenience functions for UI services
          - structures which hold information about the
            lines (disassembly, structures, enums) generated
            by the kernel
          - functions to interact with the user (dialog boxes)
          - some string and conversion functions.
*/

#ifndef __KERNWIN_HPP
#define __KERNWIN_HPP
#pragma pack(push, 1)           // IDA uses 1 byte alignments!

#ifndef SWIG
typedef uchar color_t;          ///< see <lines.hpp>
typedef uval_t bmask_t;         ///< see <enum.hpp>
typedef tid_t enum_t;           ///< see <enum.hpp>
struct areavec_t;               ///< see <area.hpp>
class location_t;               ///< see <moves.hpp>
struct strwinsetup_t;           ///< see <strlist.hpp>
#endif // SWIG

/// Message box kinds
enum mbox_kind_t
{
  mbox_internal,                ///< internal error
  mbox_info,
  mbox_warning,
  mbox_error,
  mbox_nomem,
  mbox_feedback,
  mbox_readerror,
  mbox_writeerror,
  mbox_filestruct,
  mbox_wait,
  mbox_hide,
  mbox_replace,
};


/// List chooser types
enum choose_type_t
{
  chtype_generic,                ///< the generic choose() function (1-column)
  chtype_idasgn,                 ///< see choose_idasgn()
  chtype_entry,                  ///< see choose_entry()
  chtype_name,                   ///< see choose_name()
  chtype_stkvar_xref,            ///< see choose_stkvar_xref()
  chtype_xref,                   ///< see choose_xref()
  chtype_enum,                   ///< see choose_enum()
  chtype_enum_by_value,          ///< Deprecated. See ::chtype_enum_by_value_and_size
  chtype_func,                   ///< see choose_func()
  chtype_segm,                   ///< see choose_segm()
  chtype_segreg,                 ///< Deprecated. See ::chtype_srcp
  chtype_struc,                  ///< see choose_struc()
  chtype_strpath,                ///< see choose_struc_path()
  chtype_generic2,               ///< the generic choose2() function (n-column)
  chtype_idatil,                 ///< see choose_til()
  chtype_enum_by_value_and_size, ///< see choose_enum_by_value()
  chtype_srcp,                   ///< see choose_srcp()
};


enum beep_t             ///< Beep types
{
  beep_default = 0
};


// Notify UI about various events. The kernel will call this function
// when something interesting for the UI happens.
// The UI should avoid calling the kernel from this callback.

class func_t;
class segment_t;
struct segreg_area_t;
class struc_t;
class member_t;
class TView;
class plugin_t;
class minsn_t;
class idc_value_t;
class linput_t;
class snapshot_t;

/// TCustomControl renderer type
enum tcc_renderer_type_t
{
  TCCRT_INVALID = 0,        ///< invalid
  TCCRT_FLAT,               ///< flat view
  TCCRT_GRAPH,              ///< graph view
  TCCRT_PROXIMITY           ///< proximity view
};

/// TCustomControl ::place_t type
enum tcc_place_type_t
{
  TCCPT_INVALID = 0,        ///< invalid
  TCCPT_PLACE,              ///< ::place_t
  TCCPT_SIMPLELINE_PLACE,   ///< ::simpleline_place_t
  TCCPT_IDAPLACE,           ///< ::idaplace_t
  TCCPT_ENUMPLACE,          ///< ::enumplace_t
  TCCPT_STRUCTPLACE         ///< ::structplace_t
};

/// Represents mouse button for view_mouse_event_t objects
enum vme_button_t
{
  VME_UNKNOWN,              ///< unknown mouse button
  VME_LEFT_BUTTON,          ///< left mouse button
  VME_RIGHT_BUTTON,         ///< right mouse button
  VME_MID_BUTTON,           ///< middle mouse button
};

//-------------------------------------------------------------------------
/// \defgroup SETMENU_ Set menu flags
/// Passed as 'flags' parameter to attach_action_to_menu()
//@{
#define SETMENU_POSMASK     0x1
#define SETMENU_INS         0x0 ///< add menu item before the specified path (default)
#define SETMENU_APP         0x1 ///< add menu item after the specified path
//@}

#ifndef SWIG
/// Callui return codes.
/// The size of this type should be 4 bytes at most,
/// otherwise different compilers return it differently
union callui_t
{
  bool cnd;
  char i8;
  int i;
  short i16;
  int32 i32;
  uchar u8;
  ushort u16;
  uint32 u32;
  char *cptr;
  void *vptr;
  ssize_t ssize;
  func_t *fptr;
  segment_t *segptr;
  void *sregptr;      // DEPRECATED
  struc_t *strptr;
  plugin_t *pluginptr;
  segreg_area_t *sraptr;
};

/// Events marked as '*' should be used as a parameter to callui().
/// (See convenience functions like get_screen_ea())
/// Events marked as 'cb' are designed to be callbacks and should not
/// be used in callui(). The user may hook to ::HT_UI events to catch them

enum ui_notification_t
{
  ui_null = 0,

  ui_range,             ///< cb: The disassembly range has been changed (\inf{minEA} ... \inf{maxEA}).
                        ///< UI should redraw the scrollbars. See also: ::ui_lock_range_refresh
                        ///< \param none
                        ///< \return void

  ui_list,              ///< cb: The list (chooser) window contents have been changed (names, signatures, etc).
                        ///< UI should redraw them. Please consider request_refresh() instead
                        ///< \param none
                        ///< \return void

  ui_idcstart,          ///< cb: Start of IDC engine work.
                        ///< \param none
                        ///< \return void

  ui_idcstop,           ///< cb: Stop of IDC engine work.
                        ///< \param none
                        ///< \return void

  ui_suspend,           ///< cb: Suspend graphical interface.
                        ///< Only the text version.
                        ///< Interface should respond to it.
                        ///< \param none
                        ///< \return void

  ui_resume,            ///< cb: Resume the suspended graphical interface.
                        ///< Only the text version.
                        ///< Interface should respond to it
                        ///< \param none
                        ///< \return void

  ui_old_jumpto,        ///< * Jump to the specified address.
                        ///< \param ea          (::ea_t) address to jump to
                        ///< \param operand_num (int) -1: don't change x coord
                        ///< \return bool success

  ui_readsel,           ///< * see read_selection()

  ui_unmarksel,         ///< * see unmark_selection()

  ui_screenea,          ///< * see get_screen_ea()

  ui_saving,            ///< cb: The kernel is flushing its buffers to the disk.
                        ///< The user interface should save its state.
                        ///< Parameters: none
                        ///< Returns:    none

  ui_saved,             ///< cb: The kernel has saved the database.
                        ///< This callback just informs the interface.
                        ///< \param none
                        ///< \return void

  ui_refreshmarked,     ///< * see refresh_idaview()

  ui_refresh,           ///< * see refresh_idaview_anyway()

  ui_choose,            ///< * Allow the user to choose an object.
                        ///< Always use the helper inline functions for this code.
                        ///< See \ref ui_choose_funcs for a list of such functions.
                        ///< \param type  (::choose_type_t) type of chooser to display
                        ///< \param ... other parameters depend on the given type
                        ///< \return depends on the given type

  ui_close_chooser,     ///< * see close_chooser()

  ui_banner,            ///< * see banner(int)

  ui_setidle,           ///< * Set a function to call at idle times.
                        ///< \param func  (int (*)(void)) pointer to function that will be called
                        ///< \return void

  ui_noabort,           ///< * Disable 'abort' menu item - the database was not compressed.
                        ///< \param none
                        ///< \return void

  ui_term,              ///< cb: IDA is terminated (the database is already closed).
                        ///< The UI may close its windows in this callback.
                        ///< \param none
                        ///< \return void

  ui_mbox,              ///< * Show a message box.
                        ///< \param kind    (::mbox_kind_t)
                        ///< \param format  (const char *) format of message body
                        ///< \param va      (va_list]) format args
                        ///< \return void

  ui_beep,              ///< * see beep()

  ui_msg,               ///< * Show a message in the message window. Also see ::ui_umsg.
                        ///< \param format  (const char *) format of message body
                        ///< \param va      (va_list) format args
                        ///< \return number of bytes output

  ui_askyn,             ///< * see askbuttons_c()

  ui_askfile,           ///< * see askfile_c()

  ui_form,              ///< * see \ref FORM_C

  ui_close_form,        ///< * see close_form()

  ui_clearbreak,        ///< * Clear ctrl-break flag.
                        ///< NB: this call is also used to get ida version
                        ///< \param none
                        ///< \return void

  ui_wasbreak,          ///< * see wasBreak()

  ui_asktext,           ///< * see asktext()

  ui_askstr,            ///< * see askstr()

  ui_askident,          ///< * see askident()

  ui_askaddr,           ///< * see askaddr()

  ui_askseg,            ///< * see askseg()

  ui_asklong,           ///< * see asklong()

  ui_obsolete_showauto, ///< OBSOLETE. Use showAuto()
  ui_obsolete_setstate, ///< OBSOLETE. Use setStat()

  ui_add_idckey,        ///< * see add_idc_hotkey()

/// \defgroup IDCHK_ IDC hotkey error codes
/// return values for add_idc_hotkey()
//@{
#define IDCHK_OK        0       ///< ok
#define IDCHK_ARG       -1      ///< bad argument(s)
#define IDCHK_KEY       -2      ///< bad hotkey name
#define IDCHK_MAX       -3      ///< too many IDC hotkeys
//@}

  ui_del_idckey,        ///< * see ui_del_idckey()

  ui_old_get_marker,    ///< * OBSOLETE - Get pointer to function.
                        ///< "void mark_idaview_for_refresh(ea_t ea)"
                        ///< This function will be called by the kernel when the
                        ///< database is changed
                        ///< \param none
                        ///< \return vptr: (idaapi*marker)(ea_t ea) or NULL

  ui_analyzer_options,  ///< * see analyzer_options()

  ui_is_msg_inited,     ///< * see is_msg_inited()

  ui_load_file,         ///< * see ui_load_new_file()

  ui_run_dbg,           ///< * see ui_run_debugger()

  ui_get_cursor,        ///< * see get_cursor()

  ui_get_curline,       ///< * see get_curline()

  ui_get_hwnd,          ///< * Get HWND of the main IDA window.
                        ///< \param none
                        ///< \return HWND or NULL:
                        ///<   - txt version: NULL
                        ///<   - gui version: HWND
                        ///<   - qt version under windows: HWND
                        ///<   - HWND is returned in result.vptr

  ui_copywarn,          ///< * see display_copyright_warning()

  ui_getvcl,            ///< * Get VCL variables.
                        ///< \param app     (TApplication **)
                        ///< \param screen  (TScreen **)
                        ///< \param mouse   (TMouse **)
                        ///< \return int sizeof(TApplication)+sizeof(TScreen)+sizeof(TMouse)
                        ///< The text version fills the pointers with NULLs and returns 0

  ui_idp_event,         ///< cb: A processor module event has been generated (idp.hpp, idp_notify).
                        ///< This event should not be used as a parameter to callui().
                        ///< The kernel uses it to notify the ui about the events
                        ///< \param event_mode (\ph{idp_notify})
                        ///< \param va         (va_list)
                        ///< \return int code; code==0 - process the event.
                        ///<         otherwise return code as the result

  ui_lock_range_refresh,///< * Lock the ui_range refreshes.
                        ///< The ranges will not be refreshed until the corresponding
                        ///< ::ui_unlock_range_refresh is issued.
                        ///< \param none
                        ///< \return void

  ui_unlock_range_refresh,///< * Unlock the ::ui_range refreshes.
                        ///< If the number of locks is back to zero, then refresh the ranges.
                        ///< \param none
                        ///< \return void

  ui_setbreak,          ///< * see setBreak()

  ui_genfile_callback,  ///< cb: handle html generation.
                        ///< \param html_header_cb_t **
                        ///< \param html_footer_cb_t **
                        ///< \param html_line_cb_t **
                        ///< \return void

  ui_open_url,          ///< * see open_url()

  ui_hexdumpea,         ///< * Return the current address in a hex view.
                        ///< \param result       (::ea_t *)
                        ///< \param hexdump_num  (int)
                        ///< \return void

  ui_set_xml,           ///< * set/update one or more XML values.
                        ///< \note obsolete, not supported anymore
                        ///< The 'name' element or attribute (use @XXX for an attribute)
                        ///< is created in all XML elements returned by the evaluation of the
                        ///< 'path' XPath expression, and receives the given 'value'.
                        ///< If 'name' is empty, the returned elements or attributes
                        ///< are directly updated to contain the new 'value'.
                        ///< \param path   (const char *)
                        ///< \param name   (const char *)
                        ///< \param value  (const char *)
                        ///< \return bool

  ui_get_xml,           ///< * return an XML value by evaluating the 'path' XPath expression.
                        ///< \note obsolete, not supported anymore
                        ///< \param path   (const char *)
                        ///< \param value  (::idc_value_t *)
                        ///< \return bool

  ui_del_xml,           ///< * delete XML values corresponding to the evaluation of the
                        ///< 'path' XPath expression.
                        ///< \note obsolete, not supported anymore
                        ///< \param path  (const char *)
                        ///< \return bool

  ui_push_xml,          ///< * push an XML element on a stack whose uppermost element will be
                        ///< used to evaluate future relative XPath expressions.
                        ///< \note obsolete, not supported anymore
                        ///< \param path  (const char *)
                        ///< \return bool

  ui_pop_xml,           ///< * pop the uppermost XML element from the stack.
                        ///< \note obsolete, not supported anymore
                        ///< \param none
                        ///< \return bool

  ui_get_key_code,      ///< * see get_key_code()

  ui_setup_plugins_menu,///< * setup plugins submenu
                        ///< \param none
                        ///< \return void

  ui_refresh_navband,   ///< * see refresh_navband()

  ui_new_custom_viewer, ///< * see create_custom_viewer(), also ::ui_new_ea_viewer

  ui_obsolete_add_menu_item, ///< See ::ui_register_action/::ui_attach_action_to_menu


#define SETMENU_CTXMASK     0x7C000000  ///< Deprecated. Don't use this anymore.
#define SETMENU_CTXAPP      0x00000000  ///< \copydoc SETMENU_CTXMASK
#define SETMENU_CTXIDA      0x04000000  ///< \copydoc SETMENU_CTXMASK
#define SETMENU_CTXSTR      0x08000000  ///< \copydoc SETMENU_CTXMASK
#define SETMENU_CTXENUM     0x0C000000  ///< \copydoc SETMENU_CTXMASK
#define SETMENU_CTXEA       0x10000000  ///< \copydoc SETMENU_CTXMASK
#define SETMENU_CTXVIEW     0x14000000  ///< \copydoc SETMENU_CTXMASK

  ui_obsolete_del_menu_item, ///< see ::ui_unregister_action/::ui_detach_action_from_menu

  ui_debugger_menu_change, ///< cb: debugger menu modification detected
                        ///< \param enable (bool)
                        ///< \retval true debugger menu has been added
                        ///< \retval false debugger menu will be removed

  ui_get_curplace,      ///< * see get_custom_viewer_place()

  ui_create_tform,      ///< * see create_tform()

  ui_open_tform,        ///< * see open_tform()

/// \defgroup FORM_OPEN Form open flags
/// passed as options to OpenForm_c() and open_tform()
//@{
//
#define FORM_MDI      0x01 ///< start by default as MDI
#define FORM_TAB      0x02 ///< attached by default to a tab
#define FORM_RESTORE  0x04 ///< restore state from desktop config
#define FORM_ONTOP    0x08 ///< form should be "ontop"
#define FORM_MENU     0x10 ///< form must be listed in the windows menu
                           ///< (automatically set for all plugins)
#define FORM_CENTERED 0x20 ///< form will be centered on the screen
                           ///< \return void
#define FORM_PERSIST  0x40 ///< form will persist until explicitly closed with close_tform()
                           ///< \return nothing
#define FORM_QWIDGET  0x80 ///< windows: use QWidget* instead of HWND in ::ui_tform_visible
                           ///< this flag is ignored in unix because we never use HWND there.
                           ///< around 2011/09 we plan to get rid of HWND and always use QWidget*
                           ///< regardless of this flag.
#define FORM_CLOSED_BY_ESC     0x100 ///< override idagui.cfg:CLOSED_BY_ESC: esc will close
#define FORM_NOT_CLOSED_BY_ESC 0x200 ///< override idagui.cfg:CLOSED_BY_ESC: esc will not close
//@}

  ui_close_tform,       ///< * see close_tform()

/// \defgroup FORM_CLOSE Form close flags
/// passed as options to close_tform()
//@{
#define FORM_SAVE           0x1 ///< save state in desktop config
#define FORM_NO_CONTEXT     0x2 ///< don't change the current context (useful for toolbars)
#define FORM_DONT_SAVE_SIZE 0x4 ///< don't save size of the window
#define FORM_CLOSE_LATER    0x8 ///< assign the deletion of the form to the UI loop ///< \return void
//@}

  ui_switchto_tform,    ///< * see switchto_tform()

  ui_find_tform,        ///< * see find_tform()

  ui_get_current_tform, ///< * see get_current_tform()

  ui_get_tform_handle,  ///< * Get tform handle.
                        ///< \note obsolete, not supported
                        ///< \param form  (TForm *)
                        ///< \return HWND
                        ///< tform handles can be modified by the interface           \n
                        ///< (for example, when the user switch from mdi to desktop)  \n
                        ///< This function returns the current tform handle.          \n
                        ///< It is better to hook to the 'ui_tform_visible'           \n
                        ///< event and populate the window with controls at that time.

  ui_tform_visible,     ///< TForm is displayed on the screen.
                        ///< Use this event to populate the window with controls
                        ///< \param form  (TForm *)
                        ///< \param HWND  (HWND or QWidget*)
                        ///< In unix, always work with QWigdet*
                        ///< In windows, use HWND if FORM_QWIDGET was not specified in open_tform()
                        ///< \return void

  ui_tform_invisible,   ///< TForm is being closed.
                        ///< Use this event to destroy the window controls
                        ///< params/return same as ::ui_tform_visible

  ui_get_ea_hint,       ///< cb: ui wants to display a simple hint for an address.
                        ///< Use this event to generate a custom hint
                        ///< See also more generic ::ui_get_item_hint
                        ///< \param ea       (::ea_t)
                        ///< \param buf      (char *)
                        ///< \param bufsize  (size_t)
                        ///< \return true if generated a hint

  ui_get_item_hint,     ///< cb: ui wants to display multiline hint for an item.
                        ///< See also more generic ::ui_get_custom_viewer_hint
                        ///< \param ea (ea_t, or item id like a structure or enum member)
                        ///< \param max_lines             (int) maximal number of lines
                        ///< \param[out] important_lines  (int *) number of important lines. if zero, output is ignored
                        ///< \param[out] hint             (::qstring *) the output string
                        ///< \return true if generated a hint

  ui_set_nav_colorizer, ///< * see set_nav_colorizer()

  ui_refresh_custom_viewer,
                        ///< * see refresh_custom_viewer()

  ui_destroy_custom_viewer,
                        ///< * see destroy_custom_viewer()

  ui_jump_in_custom_viewer,
                        ///< * see jumpto()

  ui_obsolete_set_custom_viewer_popup,
                        ///< see ::ui_attach_action_to_popup

  ui_obsolete_add_custom_viewer_popup,
                        ///< see ::ui_attach_action_to_popup

  ui_obsolete_set_custom_viewer_handlers,
                        ///< see ::ui_set_custom_viewer_handlers

  ui_get_custom_viewer_curline,
                        ///< * see get_custom_viewer_curline()

  ui_get_current_viewer,///< * see get_current_viewer()

  ui_is_idaview,        ///< * see is_idaview()

  ui_get_custom_viewer_hint,
                        ///< cb: ui wants to display a hint for a viewer (idaview or custom).
                        ///< \param viewer (TCustomControl*) viewer
                        ///< \param place                 (::place_t *) current position in the viewer
                        ///< \param[out] important_lines  (int *) number of important lines.
                        ///<                                     if zero, the result is ignored
                        ///< \param[out] hint             (::qstring *) the output string
                        ///< \return true if generated a hint

  ui_readsel2,          ///< * see readsel2()

  ui_set_custom_viewer_range,
                        ///< * set_custom_viewer_range()

  ui_database_inited,   ///< cb: database initialization has completed.
                        ///< the kernel is about to run idc scripts
                        ///< \param is_new_database  (int)
                        ///< \param idc_script       (const char *) - may be NULL
                        ///< \return void

  ui_ready_to_run,      ///< cb: all UI elements have been initialized.
                        ///< Automatic plugins may hook to this event to
                        ///< perform their tasks.
                        ///< \param none
                        ///< \return void

  ui_set_custom_viewer_handler,
                        ///< * see set_custom_viewer_handler().
                        ///< also see other examples in \ref ui_scvh_funcs

  ui_refresh_chooser,   ///< * see refresh_chooser()

  ui_obsolete_add_chooser_cmd,

  ui_open_builtin,      ///< * open a window of a built-in type. see \ref ui_open_builtin_funcs

  ui_preprocess,        ///< cb: ida ui is about to handle a user command.
                        ///< \param name  (const char *) ui command name.
                        ///<                            these names can be looked up in ida[tg]ui.cfg
                        ///< \retval 0 ok
                        ///< \retval nonzero a plugin has handled the command

  ui_postprocess,       ///< cb: an ida ui command has been handled

  ui_set_custom_viewer_mode,
                        ///< * switch between graph/text modes.
                        ///< \param custom_viewer  (TCustomControl *)
                        ///< \param graph_view     (bool)
                        ///< \param silent         (bool)
                        ///< \return bool success

  ui_gen_disasm_text,   ///< * see gen_disasm_text()

  ui_gen_idanode_text,  ///< cb: generate disassembly text for a node.
                        ///< Plugins may intercept this event and provide
                        ///< custom text for an IDA graph node
                        ///< They may use gen_disasm_text() for that.
                        ///< \param fc    (qflow_chart_t *)
                        ///< \param node  (int)
                        ///< \param text  (text_t *)
                        ///< \return bool text_has_been_generated

  ui_install_cli,       ///< * see:
                        ///< install_command_interpreter(),
                        ///< remove_command_interpreter()

  ui_execute_sync,      ///< * see execute_sync()

  ui_enable_input_hotkeys,
                        ///< * enable or disable alphanumeric hotkeys - they can interfere with user input.
                        ///< \note obsolete, not supported
                        ///< \param enable (bool)
                        ///< \return bool new_state

  ui_get_chooser_obj,   ///< * see get_chooser_obj()

  ui_enable_chooser_item_attrs,
                        ///< * see enable_chooser_item_attrs()

  ui_get_chooser_item_attrs,
                        ///< cb: get item-specific attributes for a chooser.
                        ///< This callback is generated only after enable_chooser_attrs()
                        ///< \param chooser_object  (void *)
                        ///< \param n               (::uint32)
                        ///< \param attrs           (::chooser_item_attrs_t *)
                        ///< \return void

  ui_set_dock_pos,      ///< * see set_dock_pos()

/// \defgroup DP_ Docking positions
/// passed as 'orient' parameter to set_dock_pos()
//@{
#define DP_LEFT            0x0001 ///< Dock src_form to the left of dest_form
#define DP_TOP             0x0002 ///< Dock src_form above dest_form
#define DP_RIGHT           0x0004 ///< Dock src_form to the right of dest_form
#define DP_BOTTOM          0x0008 ///< Dock src_form below dest_form
#define DP_INSIDE          0x0010 ///< Create a new tab bar with both src_form and dest_form
#define DP_TAB             0x0040 ///< Place src_form into a tab next to dest_form,
                                  ///< if dest_form is in a tab bar
                                  ///< (otherwise the same as #DP_INSIDE)
#define DP_BEFORE          0x0020 ///< place src_form before dst_form in the tab bar instead of after
                                  ///< used with #DP_INSIDE.
#define DP_FLOATING        0x0080 ///< Make src_form floating
//@}

  ui_get_opnum,         ///< * see get_opnum()

  ui_install_custom_datatype_menu,
                        ///< * install/remove custom data type menu item.
                        ///< \param dtid     (int) data type id
                        ///< \param install  (bool)
                        ///< \return success

  ui_install_custom_optype_menu,
                        ///< * install/remove custom operand type menu item.
                        ///< \param fid      (int) format id
                        ///< \param install  (bool)
                        ///< \return success

  ui_get_range_marker,  ///< * Get pointer to function.
                        ///< see mark_range_for_refresh(ea_t, asize_t).
                        ///< This function will be called by the kernel when the
                        ///< database is changed
                        ///< \param none
                        ///< \return vptr: (idaapi*marker)(ea_t ea, asize_t) or NULL

  ui_get_highlighted_identifier,
                        ///< * see get_highlighted_identifier()

  ui_lookup_key_code,   ///< * see lookup_key_code()

  ui_load_custom_icon_file,
                        ///< * see load_custom_icon(const char *)

  ui_load_custom_icon,  ///< * see load_custom_icon(const void *, unsigned int, const char *)

  ui_free_custom_icon,  ///< * see free_custom_icon()

  ui_process_action,    ///< * see process_ui_action()

  ui_new_code_viewer,   ///< * see create_code_viewer()

/// \defgroup CDVF_ Code viewer flags
/// passed as 'flags' parameter to create_code_viewer()
//@{
#define CDVF_NOLINES        0x0001    ///< don't show line numbers
#define CDVF_LINEICONS      0x0002    ///< icons can be drawn over the line control
#define CDVF_STATUSBAR      0x0004    ///< keep the status bar in the custom viewer
//@}

  ui_addons,            ///< * see \ref ui_addons_funcs

  ui_execute_ui_requests,
                        ///< * see execute_ui_requests(ui_request_t, ...)

  ui_execute_ui_requests_list,
                        ///< * see execute_ui_requests(ui_requests_t)

  ui_register_timer,    ///< * see register_timer()

  ui_unregister_timer,  ///< * see unregister_timer()

  ui_take_database_snapshot,
                        ///< * see take_database_snapshot()

  ui_restore_database_snapshot,
                        ///< * see restore_database_snapshot()

  ui_set_code_viewer_line_handlers,
                        ///< * see set_code_viewer_line_handlers()

  ui_refresh_custom_code_viewer,
                        ///< * Refresh custom code viewer.
                        ///< \param TCustomControl *code_viewer
                        ///< \return void

  ui_new_source_viewer, ///< * Create new source viewer.
                        ///< \param parent    (TWinControl *)
                        ///< \param custview  (TCustomControl *)
                        ///< \param path      (const char *)
                        ///< \param lines     (strvec_t *)
                        ///< \param lnnum     (int)
                        ///< \param colnum    (int)
                        ///< \param flags     (int) (\ref SVF_)
                        ///< \return source_view_t *

/// \defgroup SVF_ Source viewer creation flags
/// passed as 'flags' parameter to callback for ::ui_new_source_viewer
//@{
#define SVF_COPY_LINES  0x0000   ///< keep a local copy of '*lines'
#define SVF_LINES_BYPTR 0x0001   ///< remember the 'lines' ptr. do not make a copy of '*lines'
//@}

  ui_get_tab_size,      ///< * see get_tab_size()

  ui_set_menu_item_icon,///< * see set_menu_item_icon()

  ui_repaint_qwidget,   ///< * see repaint_custom_viewer()

  ui_enable_menu_item,  ///< * see enable_menu_item()

  ui_custom_viewer_set_userdata,
                        ///< * Change ::place_t user data for a custom view.
                        ///< \param custom_viewer  (TCustomControl *)
                        ///< \param user_data      (void *)
                        ///< \return old user_data

  ui_obsolete_new_ea_viewer,

  ui_jumpto,            ///< * see jumpto(ea_t, int, int)

  ui_choose_info,       ///< * see choose3()

  ui_cancel_exec_request,
                        ///< * see cancel_exec_request()

  ui_show_form,         ///< * see OpenForm_cv()

  ui_unrecognized_config_directive,
                        ///< * Possibly handle an extra config directive,
                        ///<   passed through '-d' or '-D'.
                        ///< \param directive  (const char *) The config directive
                        ///< \return char * - one of \ref IDPOPT_RET

  ui_obsolete_add_chooser_menu_cb,

  ui_get_viewer_name,   ///< * see get_viewer_name()

  ui_get_output_cursor, ///< * see get_output_cursor()

  ui_get_output_curline,
                        ///< * see get_output_curline()

  ui_get_output_selected_text,
                        ///< * see get_output_selected_text()

  ui_obsolete_add_output_popup, ///< see ::ui_attach_action_to_popup

  ui_get_tform_idaview, ///< * see get_tform_idaview()

  ui_get_renderer_type, ///< * see get_view_renderer_type()

  ui_set_renderer_type, ///< * see set_view_renderer_type()

  ui_askfile2,          ///< * see askfile2_c()

  ui_get_viewer_user_data,
                        ///< * see get_viewer_user_data()

  ui_get_viewer_place_type,
                        ///< * see get_viewer_place_type()

  ui_new_ea_viewer,     ///< * see create_ea_viewer()

  ui_ea_viewer_history_push_and_jump,
                        ///< * see ea_viewer_history_push_and_jump()

  ui_ea_viewer_history_info,
                        ///< * see get_ea_viewer_history_info()

  ui_register_action,
                        ///< * see register_action()

  ui_unregister_action,
                        ///< * see unregister_action()

  ui_attach_action_to_menu,
                        ///< * see attach_action_to_menu()

  ui_detach_action_from_menu,
                        ///< * see detach_action_from_menu()

  ui_attach_action_to_popup,
                        ///< * see attach_action_to_popup()

  ui_detach_action_from_popup,
                        ///< * see detach_action_from_popup()

  ui_attach_dynamic_action_to_popup,
                        ///< * see create attach_dynamic_action_to_popup()

  ui_attach_action_to_toolbar,
                        ///< * see attach_action_to_toolbar()

  ui_detach_action_from_toolbar,
                        ///< * see detach_action_from_toolbar()

  ui_updating_actions,  ///< cb: IDA is about to update all actions. If your plugin
                        ///< needs to perform expensive operations more than once
                        ///< (e.g., once per action it registers), you should do them
                        ///< only once, right away.
                        ///< \param ctx  (::action_update_ctx_t *)
                        ///< \return void

  ui_updated_actions,   ///< cb: IDA is done updating actions.
                        ///< \param none
                        ///< \return void

  ui_populating_tform_popup,
                        ///< cb: IDA is populating the context menu for a form.
                        ///< This is your chance to attach_action_to_popup().
                        ///<
                        ///< Have a look at ui_finish_populating_tform_popup,
                        ///< if you want to augment the
                        ///< context menu with your own actions after the menu
                        ///< has had a chance to be properly populated by the
                        ///< owning component or plugin (which typically does it
                        ///< on ui_populating_tform_popup.)
                        ///<
                        ///< \param form          (TForm *)
                        ///< \param popup_handle  (TPopupMenu *)
                        ///< \return void
                        ///<
                        ///< * see ui_finish_populating_tform_popup

  ui_finish_populating_tform_popup,
                        ///< cb: IDA is about to be done populating the
                        ///< context menu for a form.
                        ///< This is your chance to attach_action_to_popup().
                        ///<
                        ///< \param form          (TForm *)
                        ///< \param popup_handle  (TPopupMenu *)
                        ///< \return void
                        ///<
                        ///< * see ui_populating_tform_popup

  ui_update_action_attr,
                        ///< * see \ref ui_uaa_funcs

  ui_get_action_attr,   ///< * see \ref ui_gaa_funcs

  ui_plugin_loaded,     ///< * The plugin was loaded in memory.
                        ///< \param plugin_info  (const ::plugin_info_t *)

  ui_plugin_unloading,  ///< * The plugin is about to be unloaded
                        ///< \param plugin_info  (const ::plugin_info_t *)

  ui_get_tform_type,    ///< * see get_tform_type()

  ui_current_tform_changed,
                        ///< cb: The currently-active TForm changed.
                        ///< \param form (TForm *)
                        ///< \param prev_form (TForm *)
                        ///< \return void

  ui_get_tform_title,   ///< * see get_tform_title()

  ui_umsg,              ///< * Show a message in the message window.
                        ///< All strings ('format', and those that will be used for
                        ///< '%s' format specifiers) MUST be utf-8-encoded strings.
                        ///< See also: ::ui_msg
                        ///< \param format  (const char *)
                        ///< \param va      (va_list)
                        ///< \return number of bytes output

  ui_set_custom_viewer_handlers,
                        ///< * see set_custom_viewer_handlers()

  ui_get_user_strlist_options,
                        ///< * see get_user_strlist_options()

  ui_askqstr,           ///< * see askqstr()

  ui_last,              ///< the last notification code


  ui_dbg_begin = 1000, ///< debugger callgates. should not be used directly, see dbg.hpp for details
  ui_dbg_run_requests = ui_dbg_begin,
  ui_dbg_get_running_request,
  ui_dbg_get_running_notification,
  ui_dbg_clear_requests_queue,
  ui_dbg_get_process_state,
  ui_dbg_start_process,
  ui_dbg_request_start_process,
  ui_dbg_suspend_process,
  ui_dbg_request_suspend_process,
  ui_dbg_continue_process,
  ui_dbg_request_continue_process,
  ui_dbg_exit_process,
  ui_dbg_request_exit_process,
  ui_dbg_get_thread_qty,
  ui_dbg_getn_thread,
  ui_dbg_select_thread,
  ui_dbg_request_select_thread,
  ui_dbg_step_into,
  ui_dbg_request_step_into,
  ui_dbg_step_over,
  ui_dbg_request_step_over,
  ui_dbg_run_to,
  ui_dbg_request_run_to,
  ui_dbg_step_until_ret,
  ui_dbg_request_step_until_ret,
  ui_dbg_get_oldreg_val,                // obsolete
  ui_dbg_set_oldreg_val,                // obsolete
  ui_dbg_request_set_oldreg_val,        // obsolete
  ui_dbg_get_bpt_qty,
  ui_dbg_getn_oldbpt,                   // obsolete
  ui_dbg_get_oldbpt,                    // obsolete
  ui_dbg_add_oldbpt,                    // obsolete
  ui_dbg_request_add_oldbpt,            // obsolete
  ui_dbg_del_oldbpt,                    // obsolete
  ui_dbg_request_del_oldbpt,            // obsolete
  ui_dbg_update_oldbpt,                 // obsolete
  ui_dbg_enable_oldbpt,                 // obsolete
  ui_dbg_request_enable_oldbpt,         // obsolete
  ui_dbg_set_trace_size,
  ui_dbg_clear_trace,
  ui_dbg_request_clear_trace,
  ui_dbg_is_step_trace_enabled,
  ui_dbg_enable_step_trace,
  ui_dbg_request_enable_step_trace,
  ui_dbg_get_step_trace_options,
  ui_dbg_set_step_trace_options,
  ui_dbg_request_set_step_trace_options,
  ui_dbg_is_insn_trace_enabled,
  ui_dbg_enable_insn_trace,
  ui_dbg_request_enable_insn_trace,
  ui_dbg_get_insn_trace_options,
  ui_dbg_set_insn_trace_options,
  ui_dbg_request_set_insn_trace_options,
  ui_dbg_is_func_trace_enabled,
  ui_dbg_enable_func_trace,
  ui_dbg_request_enable_func_trace,
  ui_dbg_get_func_trace_options,
  ui_dbg_set_func_trace_options,
  ui_dbg_request_set_func_trace_options,
  ui_dbg_get_tev_qty,
  ui_dbg_get_tev_info,
  ui_dbg_get_insn_tev_oldreg_val,       // obsolete
  ui_dbg_get_insn_tev_oldreg_result,    // obsolete
  ui_dbg_get_call_tev_callee,
  ui_dbg_get_ret_tev_return,
  ui_dbg_get_bpt_tev_ea,
  ui_dbg_get_reg_value_type,
  ui_dbg_get_process_qty,
  ui_dbg_get_process_info,
  ui_dbg_attach_process,
  ui_dbg_request_attach_process,
  ui_dbg_detach_process,
  ui_dbg_request_detach_process,
  ui_dbg_get_first_module,
  ui_dbg_get_next_module,
  ui_dbg_bring_to_front,
  ui_dbg_get_current_thread,
  ui_dbg_wait_for_next_event,
  ui_dbg_get_debug_event,
  ui_dbg_set_debugger_options,
  ui_dbg_set_remote_debugger,
  ui_dbg_load_debugger,
  ui_dbg_retrieve_exceptions,
  ui_dbg_store_exceptions,
  ui_dbg_define_exception,
  ui_dbg_suspend_thread,
  ui_dbg_request_suspend_thread,
  ui_dbg_resume_thread,
  ui_dbg_request_resume_thread,
  ui_dbg_get_process_options,
  ui_dbg_check_bpt,
  ui_dbg_set_process_state,
  ui_dbg_get_manual_regions,
  ui_dbg_set_manual_regions,
  ui_dbg_enable_manual_regions,
  ui_dbg_set_process_options,
  ui_dbg_is_busy,
  ui_dbg_hide_all_bpts,
  ui_dbg_edit_manual_regions,
  ui_dbg_get_reg_val,
  ui_dbg_set_reg_val,
  ui_dbg_request_set_reg_val,
  ui_dbg_get_insn_tev_reg_val,
  ui_dbg_get_insn_tev_reg_result,
  ui_dbg_register_provider,
  ui_dbg_unregister_provider,
  ui_dbg_handle_debug_event,
  ui_dbg_add_vmod,
  ui_dbg_del_vmod,
  ui_dbg_compare_bpt_locs,
  ui_dbg_save_bpts,
  ui_dbg_old_getn_bpt,              // obsolete
  ui_dbg_old_get_bpt,               // obsolete
  ui_dbg_old_update_bpt,            // obsolete
  ui_dbg_set_bptloc_string,
  ui_dbg_get_bptloc_string,
  ui_dbg_internal_appcall,
  ui_dbg_internal_cleanup_appcall,
  ui_dbg_internal_get_sreg_base,
  ui_dbg_internal_ioctl,
  ui_dbg_read_memory,
  ui_dbg_write_memory,
  ui_dbg_read_registers,
  ui_dbg_write_register,
  ui_dbg_get_memory_info,
  ui_dbg_get_event_cond,
  ui_dbg_set_event_cond,
  ui_dbg_old_find_bpt,              // obsolete
  ui_dbg_enable_bpt,
  ui_dbg_request_enable_bpt,
  ui_dbg_old_add_bpt,               // obsolete
  ui_dbg_old_request_add_bpt,       // obsolete
  ui_dbg_del_bpt,
  ui_dbg_request_del_bpt,
  ui_dbg_map_source_path,
  ui_dbg_map_source_file_path,
  ui_dbg_modify_source_paths,
  ui_dbg_is_bblk_trace_enabled,
  ui_dbg_enable_bblk_trace,
  ui_dbg_request_enable_bblk_trace,
  ui_dbg_get_bblk_trace_options,
  ui_dbg_set_bblk_trace_options,
  ui_dbg_request_set_bblk_trace_options,
  // trace management
  ui_dbg_load_trace_file,
  ui_dbg_save_trace_file,
  ui_dbg_is_valid_trace_file,
  ui_dbg_set_trace_file_desc,
  ui_dbg_get_trace_file_desc,
  ui_dbg_choose_trace_file,
  ui_dbg_diff_trace_file,
  ui_dbg_graph_trace,
  ui_dbg_get_tev_memory_info,
  ui_dbg_get_tev_event,
  ui_dbg_get_insn_tev_reg_mem,
  // breakpoint management (new codes were introduced in v6.3)
  ui_dbg_getn_bpt,
  ui_dbg_get_bpt,
  ui_dbg_find_bpt,
  ui_dbg_add_bpt,
  ui_dbg_request_add_bpt,
  ui_dbg_update_bpt,
  ui_dbg_for_all_bpts,
  ui_dbg_get_tev_ea,
  ui_dbg_get_tev_type,
  ui_dbg_get_tev_tid,
  ui_dbg_get_tev_reg_val,
  ui_dbg_get_tev_reg_mem_qty,
  ui_dbg_get_tev_reg_mem_ea,
  ui_dbg_get_trace_base_address,
  // calluis for creating traces from scratch (added in 6.4)
  ui_dbg_set_trace_base_address,
  ui_dbg_add_tev,
  ui_dbg_add_insn_tev,
  ui_dbg_add_call_tev,
  ui_dbg_add_ret_tev,
  ui_dbg_add_bpt_tev,
  ui_dbg_add_debug_event,
  ui_dbg_add_thread,
  ui_dbg_del_thread,
  ui_dbg_add_many_tevs,
  ui_dbg_map_binary_path,
  ui_dbg_modify_binary_paths,
  ui_dbg_set_bpt_group,
  ui_dbg_set_highlight_trace_options,
  ui_dbg_set_trace_platform,
  ui_dbg_get_trace_platform,
  // added in 6.6
  ui_dbg_internal_get_elang,
  ui_dbg_internal_set_elang,

  // added in 6.7
  ui_load_dbg_dbginfo,
  ui_dbg_set_resume_mode,
  ui_dbg_request_set_resume_mode,
  ui_dbg_set_bptloc_group,
  ui_dbg_list_bptgrps,
  ui_dbg_rename_bptgrp,
  ui_dbg_del_bptgrp,
  ui_dbg_get_grp_bpts,
  ui_dbg_get_bpt_group,
  ui_dbg_change_bptlocs,

  ui_dbg_end

  // Debugging notifications
#ifdef _DEBUG
  ,

  // When execute_sync() is called from another thread,
  // make sure that thread is in an appropriate state
  // for a qsem_wait().
  //
  // IDAPython, for example, will listen to this notification, and assert that
  // the global interpreter lock does not belong to the currently-running
  // thread anymore. Otherwise, what might happen is that we have:
  //   main thread           -> sem_wait(interpreter_lock)
  //   python-created thread -> sem_wait(request.semaphore)
  // I.e., both threads are in deadlock, waiting on something
  // that will never change.
  debug_assert_thread_waitready = ui_dbg_end
#endif
};


//--------------------------------------------------------------------------
/// The user interface must call this function before calling
/// any kernel functions.
/// \param _callui  This should be the function named CALLUI
///                 The CALLUI function is a dispatcher - the user interface should have it.
/// \param argc     argc from main()
/// \param argv     argv from main()

idaman void ida_export init_kernel(
                  callui_t (idaapi*_callui)(ui_notification_t what,...),
                  int argc,
                  char *argv[]);


/// Pointer to the user-interface dispatcher function.
/// This pointer is in the kernel

idaman callui_t ida_export_data (idaapi*callui)(ui_notification_t what,...);


/// After calling init_kernel() the ui must call this function.
/// It will open the database specified in the command line.
/// If the database did not exist, a new database will be created and
/// the input file will be loaded.
/// \return 0-ok, otherwise an exit code

idaman int ida_export init_database(int argc, const char *const *argv, int *newfile);


/// The database termination function.
/// This function should be called to close the database.

idaman void ida_export term_database(void);


/// The kernel termination function.
/// This function will be automatically called when IDA exits.
/// There is no need to call it from plugins.

idaman void ida_export term_kernel(void);


/// See error()

idaman NORETURN AS_PRINTF(1, 0) void ida_export verror(const char *format, va_list va);


/// See show_hex()

idaman AS_PRINTF(3, 0) void ida_export vshow_hex(
        const void *dataptr,
        size_t len,
        const char *format,
        va_list va);


/// See show_hex_file()

idaman AS_PRINTF(4, 0) void ida_export vshow_hex_file(
        linput_t *li,
        int32 pos,
        size_t count,
        const char *format,
        va_list va);


#endif // SWIG

#define KERNEL_VERSION_MAGIC1 0x23967034
#define KERNEL_VERSION_MAGIC2 0xAAEE67BE


/// Get IDA kernel version (in a string like "5.1").
/// For old versions which do not support this call, returns false

inline bool get_kernel_version(char *buf, size_t bufsize)
{
  return callui(ui_clearbreak, KERNEL_VERSION_MAGIC1, KERNEL_VERSION_MAGIC2, buf, bufsize).cnd;
}

//--------------------------------------------------------------------------
//      K E R N E L   S E R V I C E S   F O R   U I
//--------------------------------------------------------------------------
//
// Generating text for the disassembly, enum, and structure windows.

/*! \brief Denotes a displayed line.

    (location_t would be a better name but it is too late to rename it now)

    An object may be displayed on one or more lines. All lines of an object are
    generated at once and kept in a linearray_t class.

    place_t is an abstract class, another class must be derived from it.                \n
    Currently the following classes are used in IDA:

                idaplace_t      - disassembly view                                      \n
                enumplace_t     - enum view                                             \n
                structplace_t   - structure view

    Example (idaplace_t):                                                               \verbatim

      004015AC
      004015AC loc_4015AC:                             ; CODE XREF: sub_4014B8+C5j
      004015AC                 xor     eax, eax                                         \endverbatim

    The first line is denoted by idaplace_t with ea=4015AC, lnnum=0                     \n
    The second line is denoted by idaplace_t with ea=4015AC, lnnum=1                    \n
    The third line is denoted by idaplace_t with ea=4015AC, lnnum=2

    NB: the place_t class may change in the future, do not rely on it
*/
class place_t
{
public:
  short lnnum;                      ///< Number of line within the current object
  place_t(void) {}                  ///< Constructor
  place_t(short ln) : lnnum(ln) {}  ///< Constructor
  DEFINE_MEMORY_ALLOCATION_FUNCS()

  /// Generate a short description of the location.
  /// This description is used on the status bar.
  /// \param ud       pointer to user-defined context data. Is supplied by ::linearray_t
  /// \param out_buf  the output buffer
  /// \param bufsize  size of the output buffer
  virtual void idaapi print(void *ud, char *out_buf, size_t bufsize) const  = 0;

  /// Map the location to a number.
  /// This mapping is used to draw the vertical scrollbar.
  /// \param ud  pointer to user-defined context data. Is supplied by ::linearray_t
  virtual uval_t idaapi touval(void *ud) const                         = 0;

  /// Clone the location.
  /// \return a pointer to a copy of the current location in dynamic memory
  virtual place_t *idaapi clone(void) const                            = 0;

  /// Copy the specified location object to the current object
  virtual void idaapi copyfrom(const place_t *from)                    = 0;

  /// Map a number to a location.
  /// When the user clicks on the scrollbar and drags it, we need to determine
  /// the location corresponding to the new scrollbar position. This function
  /// is used to determine it. It builds a location object for the specified 'x'
  /// and returns a pointer to it.
  /// \param ud     pointer to user-defined context data. Is supplied by ::linearray_t
  /// \param x      number to map
  /// \param lnnum  line number to initialize 'lnnum'
  /// \return a static object, no need to destroy it.
  virtual place_t *idaapi makeplace(void *ud, uval_t x, short lnnum) const= 0;

  /// Compare two locations except line numbers (lnnum).
  /// This function is used to organize loops.
  /// For example, if the user has selected an area, its boundaries are remembered
  /// as location objects. Any operation within the selection will have the following
  /// look: for ( loc=starting_location; loc < ending_location; loc.next() )
  /// In this loop, the comparison function is used.
  /// \retval -1 if the current location is less than 't2'
  /// \retval  0 if the current location is equal to than 't2'
  /// \retval  1 if the current location is greater than 't2'
  virtual int idaapi compare(const place_t *t2) const                  = 0;

  /// Adjust the current location to point to a displayable object.
  /// This function validates the location and makes sure that it points to
  /// an existing object. For example, if the location points to the middle
  /// of an instruction, it will be adjusted to point to the beginning of the
  /// instruction.
  /// \param ud  pointer to user-defined context data. Is supplied by ::linearray_t
  virtual void idaapi adjust(void *ud)                                 = 0;

  /// Move to the previous displayable location.
  /// \param ud  pointer to user-defined context data. Is supplied by ::linearray_t
  /// \return success
  virtual bool idaapi prev(void *ud)                                   = 0;

  /// Move to the next displayable location.
  /// \param ud  pointer to user-defined context data. Is supplied by ::linearray_t
  /// \return success
  virtual bool idaapi next(void *ud)                                   = 0;

  /// Are we at the first displayable object?.
  /// \param ud   pointer to user-defined context data. Is supplied by ::linearray_t
  /// \return true if the current location points to the first displayable object
  virtual bool idaapi beginning(void *ud) const                        = 0;

  /// Are we at the last displayable object?.
  /// \param ud   pointer to user-defined context data. Is supplied by ::linearray_t
  /// \return true if the current location points to the last displayable object
  virtual bool idaapi ending(void *ud) const                           = 0;

  /// Generate text lines for the current location.
  /// \param ud             pointer to user-defined context data. Is supplied by linearray_t
  /// \param lines          array of pointers to output lines. the pointers will be overwritten
  ///                       by lines that are allocated using qstrdup. the caller must qfree them.
  /// \param maxsize        maximal number of lines to generate
  /// \param default_lnnum  pointer to the cell that will contain the number of
  ///                       the most 'interesting' generated line
  /// \param pfx_color      pointer to the cell that will contain the line prefix color
  /// \param bgcolor        pointer to the cell that will contain the background color
  /// \return number of generated lines
  virtual int idaapi generate(
        void *ud,
        char *lines[],
        int maxsize,
        int *default_lnnum,
        color_t *pfx_color,
        bgcolor_t *bgcolor) const                = 0;
};

#ifndef SWIG
/// compare places and their lnnums
idaman int ida_export l_compare(const place_t *t1, const place_t *t2);

//--------------------------------------------------------------------------
/// Helper to define exported functions for ::place_t implementations
#define define_place_exported_functions(classname)                                  \
class classname;                                                                    \
idaman void     ida_export classname ## __print(const classname *,void*,char*, size_t);    \
idaman uval_t   ida_export classname ## __touval(const classname *,void*);                 \
idaman place_t *ida_export classname ## __clone(const classname *);                        \
idaman void     ida_export classname ## __copyfrom(classname *,const place_t*);            \
idaman place_t *ida_export classname ## __makeplace(const classname *,void*,uval_t,short); \
idaman int      ida_export classname ## __compare(const classname *,const place_t*);       \
idaman void     ida_export classname ## __adjust(classname *,void*);                       \
idaman bool     ida_export classname ## __prev(classname *,void*);                         \
idaman bool     ida_export classname ## __next(classname *,void*);                         \
idaman bool     ida_export classname ## __beginning(const classname *,void*);              \
idaman bool     ida_export classname ## __ending(const classname *,void*);                 \
idaman int      ida_export classname ## __generate(const classname *,void*,char**,int,int*,color_t*,bgcolor_t*);

/// Helper to define virtual functions in ::place_t implementations
#define define_place_virtual_functions(class)                           \
  void idaapi print(void *ud,char *buf, size_t bufsize) const           \
        {        class ## __print(this,ud,buf,bufsize); }               \
  uval_t idaapi touval(void *ud) const                                  \
        { return class ## __touval(this,ud); }                          \
  place_t *idaapi clone(void) const                                     \
        { return class ## __clone(this); }                              \
  void idaapi copyfrom(const place_t *from)                             \
        {        class ## __copyfrom(this,from); }                      \
  place_t *idaapi makeplace(void *ud,uval_t x,short _lnnum) const       \
        { return class ## __makeplace(this,ud,x,_lnnum); }              \
  int  idaapi compare(const place_t *t2) const                          \
        { return class ## __compare(this,t2); }                         \
  void idaapi adjust(void *ud)                                          \
        {        class ## __adjust(this,ud); }                          \
  bool idaapi prev(void *ud)                                            \
        { return class ## __prev(this,ud); }                            \
  bool idaapi next(void *ud)                                            \
        { return class ## __next(this,ud); }                            \
  bool idaapi beginning(void *ud) const                                 \
        { return class ## __beginning(this,ud); }                       \
  bool idaapi ending (void *ud) const                                   \
        { return class ## __ending(this,ud); }                          \
  int idaapi generate (void *ud,char *lines[],int maxsize,int *_lnnum,  \
                       color_t *pfx_color, bgcolor_t *bg_color) const   \
        { return class ## __generate(this,ud,lines,maxsize,_lnnum,      \
                                                pfx_color, bg_color); }

define_place_exported_functions(simpleline_place_t)
#endif // SWIG

//--------------------------------------------------------------------------

/*! \defgroup simpleline Simpleline interface

  \brief IDA custom viewer sample.

  It is enough to create an object of ::strvec_t class, put all lines
  into it and create a custom ida viewer (::ui_new_custom_viewer).
                                                                     \code
    strvec_t sv;
    // fill it with lines...
    simpleline_place_t s1;
    simpleline_place_t s2(sv.size()-1);
    cv = (TCustomControl *)callui(ui_new_custom_viewer,
                           "My title",
                           parent_form,
                           &s1,
                           &s2,
                           &s1,
                           0,
                           &sv).vptr;
                                                                     \endcode
  This will produce a nice colored text view.
  Also see the SDK's 'custview' and 'hexview' plugins for more complete examples.
*/
//@{

/// Maintain basic information for a line in a custom view
struct simpleline_t
{
  qstring line;       ///< line text
  color_t color;      ///< line prefix color
  bgcolor_t bgcolor;  ///< line background color
  simpleline_t(void) : color(1), bgcolor(DEFCOLOR) {}                                   ///< Constructor (default colors)
  simpleline_t(color_t c, const char *str) : line(str), color(c), bgcolor(DEFCOLOR) {}  ///< Constructor
  simpleline_t(const char *str) : line(str), color(1), bgcolor(DEFCOLOR) {}             ///< Constructor
  simpleline_t(const qstring &str) : line(str), color(1), bgcolor(DEFCOLOR) {}          ///< Constructor
  DEFINE_MEMORY_ALLOCATION_FUNCS()
};

/// A collection of simple lines to populate a custom view.
/// This is an example of what you would pass as the 'ud' argument to create_custom_viewer()
typedef qvector<simpleline_t> strvec_t;

/// A location in a view populated by a ::strvec_t
class simpleline_place_t : public place_t
{
public:
  size_t n; ///< line number
  simpleline_place_t(void) { n = 0; lnnum = 0; }    ///< Constructor
  simpleline_place_t(int _n) { n = _n; lnnum = 0; } ///< Constructor
  define_place_virtual_functions(simpleline_place_t);
};
//@}

//--------------------------------------------------------------------------
// user defined data for linearray_t: use ptr to result of calc_default_idaplace_flags()
#ifndef SWIG
define_place_exported_functions(idaplace_t)
#endif // SWIG
/// A location in a disassembly view
class idaplace_t : public place_t {
public:
  ea_t ea; ///< address
  idaplace_t(void) {} ///< Constructor
  idaplace_t(ea_t x,short ln) : place_t(ln), ea(x) {} ///< Constructor
  define_place_virtual_functions(idaplace_t);
};

//--------------------------------------------------------------------------
// user defined data for linearray_t: NULL
#ifndef SWIG
define_place_exported_functions(enumplace_t)
#endif // SWIG
/// A location in an enum view
class enumplace_t : public place_t {
public:
  size_t idx;           ///< enum serial number
  bmask_t bmask;        ///< enum member bitmask
  uval_t value;         ///< enum member value
  uchar serial;         ///< enum member serial number
  enumplace_t(void) {}  ///< Constructor
  enumplace_t(size_t i, bmask_t m, uval_t v, uchar s, short ln) ///< Constructor
    : place_t(ln), idx(i), bmask(m), value(v), serial(s) {}
  define_place_virtual_functions(enumplace_t);
};

//--------------------------------------------------------------------------
// user defined data for linearray_t: ea_t *pea
// if pea != NULL then the function stack frame is displayed, *pea == function start
// else                normal structure list is displayed
#ifndef SWIG
define_place_exported_functions(structplace_t)
#endif // SWIG
/// A location in a struct view
class structplace_t : public place_t {
public:
  uval_t idx;             ///< struct serial number
  uval_t offset;          ///< offset within struct
  structplace_t(void) {}  ///< Constructor
  structplace_t(uval_t i, uval_t o, short ln) : place_t(ln), idx(i), offset(o) {} ///< Constructor
  define_place_virtual_functions(structplace_t);
};

#ifndef SWIG
/// The saved struct view position in the database
struct saved_structplace_t
{
  ushort lnnum;  ///< line number
  ushort x,y;    ///< cursor position
  uval_t idx;    ///< struct serial number
  uval_t offset; ///< offset within struct
};
#endif // SWIG

//----------------------------------------------------------------------
/// A position in a text window
class twinpos_t
{
public:
  place_t *at;                                    ///< location in view
  int x;                                          ///< cursor x
  twinpos_t(void)                 {}              ///< Constructor
  twinpos_t(place_t *t)           { at=t; }       ///< Constructor
  twinpos_t(place_t *t,int x0)    { at=t; x=x0; } ///< Constructor
  DEFINE_MEMORY_ALLOCATION_FUNCS()
  /// compare two twinpos_t's with '!='
  bool operator != (const twinpos_t &r)
  {
    if ( x != r.x ) return true;
    if ( (at == NULL) != (r.at == NULL) ) return true;
    if ( at != NULL && (at->compare(r.at) != 0 || at->lnnum != r.at->lnnum) ) return true;
    return false;
  }
  /// compare two twinpos_t's with '=='
  bool operator == (const twinpos_t &r) { return !(*this != r); }
};

#ifndef SWIG
/// A line in a text window
class twinline_t
{
public:
  place_t *at;             ///< location in view
  char *line;              ///< line contents
  color_t prefix_color;    ///< line prefix color
  bgcolor_t bg_color;      ///< line background color
  bool is_default;         ///< is this the default line of the current location?
  twinline_t(void) {}      ///< Constructor
  twinline_t(place_t *t,char *l,color_t pc, bgcolor_t bc) ///< Constructor
  {
    at           = t;
    line         = l;
    prefix_color = pc;
    bg_color     = bc;
    is_default   = false;
  }
  DEFINE_MEMORY_ALLOCATION_FUNCS()
};

/// A group of lines in a text window
typedef qvector<twinline_t> text_t;
/// A collection of subdivisions of a text window
typedef qvector<text_t> texts_t;

/// Helper for declaring member functions of the ::linearray_t class
#define DECLARE_LINEARRAY_HELPERS(decl) \
decl void  ida_export linearray_t_ctr      (linearray_t *, void *ud); \
decl void  ida_export linearray_t_dtr      (linearray_t *); \
decl int   ida_export linearray_t_set_place(linearray_t *, const place_t *new_at); \
decl bool  ida_export linearray_t_beginning(const linearray_t *); \
decl bool  ida_export linearray_t_ending   (const linearray_t *); \
decl char *ida_export linearray_t_down     (linearray_t *); \
decl char *ida_export linearray_t_up       (linearray_t *);

class linearray_t;
DECLARE_LINEARRAY_HELPERS(idaman)

/// The group of lines corresponding to a single place within a view
class linearray_t
{
  DECLARE_LINEARRAY_HELPERS(friend)
  int   _set_place(const place_t *new_at);
  char *_down     (void);
  char *_up       (void);

  char **lines;                 // [lnar_size]; we own all these lines
  place_t *at;
  void *ud;                     // user defined data (UD)
                                // its meaning depends on the place_t used
  int linecnt;                  // number of lines
  color_t prefix_color;         // prefix color
  bgcolor_t bg_color;           // background color
  char *extra;                  // the last line of the previous location after moving down
  int dlnnum;                   // default line number (if unknown, -1)

  int   getlines        (void);
  void  cleanup         (void);

public:

  linearray_t(void *_ud)                     { linearray_t_ctr(this, _ud); } ///< Constructor
  ~linearray_t(void)                         { linearray_t_dtr(this); }      ///< Constructor
  DEFINE_MEMORY_ALLOCATION_FUNCS()

  /// Position the array.
  /// This function must be called before calling any other member functions.
  ///
  /// ::linearray_t doesn't own ::place_t structures.
  /// The caller must take care of place_t objects.
  ///
  /// \param new_at  new position of the array
  /// \return the delta of lines that the linearray_t had to adjust the place by.             \n
  /// For example, if the place_t has a lnnum of 5, but it turns out, upon generating lines,  \n
  /// that the number of lines for that particular place is only 2, then 3 will be returned.
  int set_place(const place_t *new_at)      { return linearray_t_set_place(this, new_at); }

  /// Get the current place.
  /// If called before down(), then returns place of line which will be returned by down().
  /// If called after up(), then returns place if line returned by up().
  place_t* get_place    (void) const         { return at; }

  /// Get current background color.
  /// (the same behavior as with get_place(): good before down() and after up())
  bgcolor_t get_bg_color(void) const         { return bg_color; }

  /// Get current prefix color.
  /// (the same behavior as with get_place(): good before down() and after up())
  bgcolor_t get_pfx_color(void) const        { return prefix_color; }

  /// Get default line number.
  /// (the same behavior as with get_place(): good before down() and after up())
  int   get_dlnnum      (void) const         { return dlnnum; }

  /// Get number of lines for the current place.
  /// (the same behavior as with get_place(): good before down() and after up())
  int   get_linecnt     (void) const         { return linecnt; }

  /// Get pointer to user data
  void *userdata        (void) const         { return ud; }

  /// Change the user data
  void set_userdata     (void *userd)        { ud = userd; }

  /// Are we at the beginning?
  bool beginning(void) const                 { return linearray_t_beginning(this); }

  // Are we at the end?
  bool ending(void) const                    { return linearray_t_ending(this); }

  /// Get a line from down direction.
  /// place is ok BEFORE
  char *down(void)
        { return linearray_t_down(this); }

  /// Get a line from up direction.
  /// place is ok AFTER
  char *up(void)
        { return linearray_t_up(this); }

};
#endif // SWIG


/// Bitmask of builtin window types to be refreshed:
idaman unsigned int ida_export_data dirty_infos;


/// Request a refresh of a builtin window.
/// \param mask  \ref IWID_

inline void request_refresh(unsigned int mask)
{
  dirty_infos |= mask;
}

//-------------------------------------------------------------------------
typedef int tform_type_t; ///< \ref BWN_

/// \defgroup BWN_ Window types
/// also see \ref ui_open_builtin_funcs
//@{
#define BWN_UNKNOWN    -1 ///< unknown window
#define BWN_EXPORTS     0 ///< exports
#define BWN_IMPORTS     1 ///< imports
#define BWN_NAMES       2 ///< names
#define BWN_FUNCS       3 ///< functions
#define BWN_STRINGS     4 ///< strings
#define BWN_SEGS        5 ///< segments
#define BWN_SEGREGS     6 ///< segment registers
#define BWN_SELS        7 ///< selectors
#define BWN_SIGNS       8 ///< signatures
#define BWN_TILS        9 ///< type libraries
#define BWN_LOCTYPS    10 ///< local types
#define BWN_CALLS      11 ///< function calls
#define BWN_PROBS      12 ///< problems
#define BWN_BPTS       13 ///< breakpoints
#define BWN_THREADS    14 ///< threads
#define BWN_MODULES    15 ///< modules
#define BWN_TRACE      16 ///< trace view
#define BWN_CALL_STACK 17 ///< call stack
#define BWN_XREFS      18 ///< xrefs
#define BWN_SEARCH     19 ///< search results
#define BWN_FRAME      25 ///< function frame
#define BWN_NAVBAND    26 ///< navigation band
#define BWN_ENUMS      27 ///< enumerations
#define BWN_STRUCTS    28 ///< structures
#define BWN_DISASM     29 ///< disassembly views
#define BWN_DUMP       30 ///< hex dumps
#define BWN_NOTEPAD    31 ///< notepad
// The following cannot be requested to be refreshed,
// but can still be useful to get their type through
// get_tform_type().
#define BWN_OUTPUT      32 ///< the text area, in the output window
#define BWN_CLI         33 ///< the command-line, in the output window
#define BWN_WATCH       34 ///< the 'watches' debugger window
#define BWN_LOCALS      35 ///< the 'locals' debugger window
#define BWN_STKVIEW     36 ///< the 'Stack view' debugger window
#define BWN_CHOOSER     37 ///< a non-builtin chooser
#define BWN_SHORTCUTCSR 38 ///< the shortcuts chooser (Qt version only)
#define BWN_SHORTCUTWIN 39 ///< the shortcuts window (Qt version only)
#define BWN_CPUREGS     40 ///< one of the 'General registers', 'FPU register', ... debugger windows
#define BWN_SO_STRUCTS  41 ///< the 'Structure offsets' dialog's 'Structures and Unions' panel
#define BWN_SO_OFFSETS  42 ///< the 'Structure offsets' dialog's offset panel
/// Alias. Some BWN_* were confusing, and thus have been renamed.
/// This is to ensure bw-compat.
#define BWN_STACK   BWN_CALL_STACK
#define BWN_DISASMS BWN_DISASM  ///< \copydoc BWN_STACK
#define BWN_DUMPS   BWN_DUMP    ///< \copydoc BWN_STACK
#define BWN_SEARCHS BWN_SEARCH  ///< \copydoc BWN_STACK
//@}

/// \defgroup IWID_ Window refresh flags
/// passed as 'mask' parameter to request_refresh()
//@{
#define IWID_EXPORTS  (1u << BWN_EXPORTS) ///< exports           (0)
#define IWID_IMPORTS  (1u << BWN_IMPORTS) ///< imports           (1)
#define IWID_NAMES    (1u << BWN_NAMES  ) ///< names             (2)
#define IWID_FUNCS    (1u << BWN_FUNCS  ) ///< functions         (3)
#define IWID_STRINGS  (1u << BWN_STRINGS) ///< strings           (4)
#define IWID_SEGS     (1u << BWN_SEGS   ) ///< segments          (5)
#define IWID_SEGREGS  (1u << BWN_SEGREGS) ///< segment registers (6)
#define IWID_SELS     (1u << BWN_SELS   ) ///< selectors         (7)
#define IWID_SIGNS    (1u << BWN_SIGNS  ) ///< signatures        (8)
#define IWID_TILS     (1u << BWN_TILS   ) ///< type libraries    (9)
#define IWID_LOCTYPS  (1u << BWN_LOCTYPS) ///< local types       (10)
#define IWID_CALLS    (1u << BWN_CALLS  ) ///< function calls    (11)
#define IWID_PROBS    (1u << BWN_PROBS  ) ///< problems          (12)
#define IWID_BPTS     (1u << BWN_BPTS   ) ///< breakpoints       (13)
#define IWID_THREADS  (1u << BWN_THREADS) ///< threads           (14)
#define IWID_MODULES  (1u << BWN_MODULES) ///< modules           (15)
#define IWID_TRACE    (1u << BWN_TRACE  ) ///< trace view        (16)
#define IWID_STACK    (1u << BWN_STACK  ) ///< call stack        (17)
#define IWID_XREFS    (1u << BWN_XREFS  ) ///< xrefs             (18)
#define IWID_SEARCHS  (1u << BWN_SEARCH ) ///< search results    (19)
#define IWID_FRAME    (1u << BWN_FRAME  ) ///< function frame    (25)
#define IWID_NAVBAND  (1u << BWN_NAVBAND) ///< navigation band   (26)
#define IWID_ENUMS    (1u << BWN_ENUMS  ) ///< enumerations      (27)
#define IWID_STRUCTS  (1u << BWN_STRUCTS) ///< structures        (28)
#define IWID_DISASMS  (1u << BWN_DISASM ) ///< disassembly views (29)
#define IWID_DUMPS    (1u << BWN_DUMP   ) ///< hex dumps         (30)
#define IWID_NOTEPAD  (1u << BWN_NOTEPAD) ///< notepad           (31)
#define IWID_IDAMEMOS (IWID_DISASMS|IWID_DUMPS)
                                          ///< disassembly + hex dump views
#define IWID_ALL     0xFFFFFFFF           ///< mask
//@}


/// Does the given tform type specify a chooser tform?

inline bool is_chooser_tform(tform_type_t t)
{
  return t == BWN_CHOOSER
      || (t >= BWN_EXPORTS && t <= BWN_SEARCH && t != BWN_CALLS)
      || t == BWN_SHORTCUTCSR;
}


#ifndef SWIG
//---------------------------------------------------------------------------
//      D E B U G G I N G   F U N C T I O N S
//---------------------------------------------------------------------------

/// Controls debug messages - combination of \ref IDA_DEBUG_
idaman uint32 ida_export_data debug;

/// \defgroup IDA_DEBUG_ IDA debug bits
/// used by ::debug
//@{
#define IDA_DEBUG_DREFS         0x00000001      ///< drefs
#define IDA_DEBUG_OFFSET        0x00000002      ///< offsets
#define IDA_DEBUG_FLIRT         0x00000004      ///< flirt
#define IDA_DEBUG_IDP           0x00000008      ///< idp module
#define IDA_DEBUG_LDR           0x00000010      ///< ldr module
#define IDA_DEBUG_PLUGIN        0x00000020      ///< plugin module
#define IDA_DEBUG_IDS           0x00000040      ///< ids files
#define IDA_DEBUG_CONFIG        0x00000080      ///< config file
#define IDA_DEBUG_CHECKMEM      0x00000100      ///< check heap consistency
#define IDA_DEBUG_CHECKARG      0x00000200      ///< checkarg
#define IDA_DEBUG_DEMANGLE      0x00000400      ///< demangler
#define IDA_DEBUG_QUEUE         0x00000800      ///< queue
#define IDA_DEBUG_ROLLBACK      0x00001000      ///< rollback
#define IDA_DEBUG_ALREADY       0x00002000      ///< already data or code
#define IDA_DEBUG_TIL           0x00004000      ///< type system
#define IDA_DEBUG_NOTIFY        0x00008000      ///< show all notifications
#define IDA_DEBUG_DEBUGGER      0x00010000      ///< debugger
#define IDA_DEBUG_APPCALL       0x00020000      ///< appcall
#define IDA_DEBUG_SRCDBG        0x00040000      ///< source debugging
#define IDA_DEBUG_ACCESSIBILITY 0x00080000      ///< accessibility
#define IDA_DEBUG_ALWAYS        0xFFFFFFFF      ///< everything
//@}


/// Display debug message.
/// \param ida_debug_bits  \ref IDA_DEBUG_, also see ::debug
/// \param format           printf()-style format
/// \return number of bytes output

AS_PRINTF(2, 3) inline int deb(int ida_debug_bits, const char *format, ...)
{
  int nbytes = 0;
  if ( debug & ida_debug_bits )
  {
    va_list va;
    va_start(va, format);
    nbytes = callui(ui_msg, format, va).i;
    va_end(va);
  }
  return nbytes;
}

#ifdef __NT__
/// Print the tick count from the last call to debug_time().
/// The first time prints -1.
#define debug_time()  ida_debug_time(__FILE__, __LINE__)
/// See debug_time()
void ida_debug_time(const char *file, int line);
#else
#define debug_time()
#endif


/// Checking heap is not available anymore.
#define checkmem() do {} while (0) // ida_checkmem(__FILE__, __LINE__)
/// \copydoc checkmem()
idaman void ida_export ida_checkmem(const char *file, int line);


/// Display hex dump in the messages window

AS_PRINTF(3, 4) inline void show_hex(
        const void *dataptr,
        size_t len,
        const char *format,
        ...)
{
  va_list va;
  va_start(va,format);
  vshow_hex(dataptr, len, format, va);
  va_end(va);
}


/// Display hex dump of a file in the messages window

AS_PRINTF(4, 5) inline void show_hex_file(
        linput_t *li,
        int32 pos,
        size_t count,
        const char *format,
        ...)
{
  va_list va;
  va_start(va,format);
  vshow_hex_file(li, pos, count, format, va);
  va_end(va);
}
#endif // SWIG

//-------------------------------------------------------------------------
//      U I   S E R V I C E  F U N C T I O N S
//-------------------------------------------------------------------------

#ifndef SWIG
// Common function prototypes
// These functions are inlined for the kernel
// They are not inlined for the user-interfaces

AS_PRINTF(5, 0) int askbuttons_cv(
        const char *Yes,
        const char *No,
        const char *Cancel,
        int deflt,
        const char *format,
        va_list va);
AS_PRINTF(3, 0) char *askfile_cv(
        int savefile,
        const char *defval,
        const char *format,
        va_list va);
AS_PRINTF(4, 0) char *askfile2_cv(
        bool forsave,
        const char *defdir,
        const char *filters,
        const char *format,
        va_list va);
AS_PRINTF(3, 0) char *vaskstr(
        int hist,
        const char *defval,
        const char *format,
        va_list va);
AS_PRINTF(2, 0) bool vaskqstr(
  qstring *str,
  const char *format,
  va_list va);
AS_PRINTF(4, 0) char *vasktext(
        size_t size,
        char *answer,
        const char *defval,
        const char *format,
        va_list va);
AS_PRINTF(1, 0) int vmsg(const char *format, va_list va);
AS_PRINTF(1, 0) int vumsg(const char *format, va_list va);
AS_PRINTF(1, 0) void vwarning(const char *format, va_list va);
AS_PRINTF(1, 0) void vinfo(const char *format, va_list va);
AS_PRINTF(1, 0) NORETURN void vnomem(const char *format, va_list va);


/// \defgroup CH_ Generic chooser flags
/// used by 'flags' parameter for \ref choosers and also chooser_info_t::flags
//@{
#define CH_MODAL 0x01 ///< Modal chooser

#define CH_MULTI 0x02 ///< The chooser will allow multi-selection (only for GUI choosers).
                      ///< If multi-selection is enabled, a multi-selection 'del' callback
                      ///< will be called with:
                      ///<   - ::uint32 = ::START_SEL   before calling the first selected item
                      ///<   - ::uint32 =  1..n       for each selected item
                      ///<   - ::uint32 = ::END_SEL     after calling the last  selected item
                      ///<
                      ///< If the callback returns 'false', further processing
                      ///<   of selected items is cancelled.

#define CH_MULTI_EDIT 0x04
                      ///< The 'edit' callback will be called for all selected items
                      ///< using the ::START_SEL/::END_SEL protocol.
                      ///< This bit implies #CH_MULTI (valid only for gui)

#define CH_NOBTNS 0x08 ///< do not display ok/cancel/help/search buttons.
                      ///< meaningful only for gui modal windows because non-modal
                      ///< windows do not have any buttons anyway. text mode does
                      ///< not have them neither

#define CH_ATTRS 0x10 ///< generate ui_get_chooser_item_attrs (gui only)
#define CH_NOIDB 0x20 ///< use the chooser even without an open database, same as x0=-2 (see below)
#define CH_UTF8  0x40 ///< string encoding is utf-8
#define CH_BUILTIN_MASK 0xF80000
                      ///< Mask for builtin chooser numbers. Plugins should not use them
#define CH_BUILTIN(id) ((id+1) << 19)
#define CH_GETCN(flags) (((flags & CH_BUILTIN_MASK) >> 19) - 1)
//@}

/// \defgroup CHCOL_ Chooser column flags
/// used by 'widths' parameter for \ref choosers
//@{
#define CHCOL_PLAIN     0x00000000  ///< plain string
#define CHCOL_PATH      0x00010000  ///< file path
#define CHCOL_HEX       0x00020000  ///< hexadecimal number
#define CHCOL_DEC       0x00030000  ///< decimal number
#define CHCOL_FORMAT    0x00070000  ///< column format mask
//@}
#endif // SWIG


typedef int32 chooser_event_t; ///< see \ref _SEL

/// \defgroup _SEL Chooser selections
/// chooser multi-selection callback special events (passed as indexes to callbacks).
//@{
const chooser_event_t
  EMPTY_SEL =  0, ///< no item was selected
  START_SEL = -1, ///< before calling the first selected item
  END_SEL   = -2; ///< after calling the last selected item

// chooser multi-selection callback convenience macros
#define IS_EMPTY_SEL(n)     ((chooser_event_t)(size_t)n == EMPTY_SEL)
#define IS_START_SEL(n)     ((chooser_event_t)(size_t)n == START_SEL)
#define IS_END_SEL(n)       ((chooser_event_t)(size_t)n == END_SEL)
#define IS_CHOOSER_EVENT(n) (IS_EMPTY_SEL(n) \
                          || IS_START_SEL(n) \
                          || IS_END_SEL(n))
#define IS_SEL(n)           (!IS_CHOOSER_EVENT(n))
//@}

#ifndef SWIG
/// \name Chooser title
/// prefixes to be used in the chooser title
//@{
#define CHOOSER_NOMAINMENU  "NOMAINMENU\n"   ///< do not display main menu
#define CHOOSER_NOSTATUSBAR "NOSTATUSBAR\n"  ///< do not display status bar
//@}

/// Callback passed to functions in \ref choosers
typedef uint32 idaapi chooser_cb_t(void *obj, uint32 n);

/// Chooser item attributes
struct chooser_item_attrs_t
{
  int cb;               ///< size of this structure.
                        ///< the callback must check this field and fill only
                        ///< the existing fields. the first 2 fields always exist:
  int flags;            ///< \ref CHITEM_
/// \defgroup CHITEM_ Chooser item property bits
/// used by chooser_item_attrs_t::flags
//@{
#define CHITEM_BOLD   0x0001 ///< display the item in bold
#define CHITEM_ITALIC 0x0002 ///< display the item in italic
#define CHITEM_UNDER  0x0004 ///< underline the item
#define CHITEM_STRIKE 0x0008 ///< strikeout the item
#define CHITEM_GRAY   0x0010 ///< gray out the item
//@}
  bgcolor_t color;      ///< item color
  void reset(void)      ///< restore to defaults
  {
    cb    = sizeof(chooser_item_attrs_t);
    flags = 0;
    color = DEFCOLOR;
  }
};

/// Chooser structure, used to embed a chooser in a form.
struct chooser_info_t
{
  uint32 cb;          ///< size of the struct

  int flags;          ///< \ref CH_
  int width, height;  ///< in chars
  const char *title;  ///< title

  void *obj;          ///< data passed to callback functions
  int columns;        ///< number of columns
  const int *widths;  ///< column widths

  int icon;           ///< default icon
  uint32 deflt;       ///< default selection

  const char *const *popup_names;   ///< array of custom popup menu names.
                                    ///< Used replace labels for the standard handlers
                                    ///< (Insert, Delete, Edit, Refresh).                 \n
                                    ///< If less than four custom menu items are needed.  \n
                                    ///< The array can be NULL-terminated

  /// Callback - get the number of elements in the chooser
  uint32 (idaapi *sizer)(void *obj);

  /// Callback - get a description of an element.
  /// \param obj          chooser_info_t::obj
  /// \param n            element number
  /// \param[out] arrptr  pointer to array of strings. \n
  ///                     will receive the contents of each column
  void (idaapi *getl)(void *obj, uint32 n, char *const *arrptr);

  /// Callback - user deleted an element.
  /// \param obj  chooser_info_t::obj
  /// \param n    number of deleted element
  /// \return index of element that took its place
  uint32 (idaapi *del)(void *obj, uint32 n);

  /// Callback - user asked to insert an element (may be NULL).
  /// \param obj  chooser_info_t::obj
  void (idaapi *ins)(void *obj);

  /// Callback - the chooser needs to be refreshed (may be NULL).
  /// \param obj  chooser_info_t::obj
  /// \param n    currently selected element
  /// \return the new location of item 'n'
  uint32 (idaapi *update)(void *obj, uint32 n);

  /// Callback - user asked to edit an element (may be NULL).
  /// \param obj  chooser_info_t::obj
  /// \param n    element to edit
  void (idaapi *edit)(void *obj, uint32 n);

  /// Callback - user pressed the enter key (may be NULL).
  /// \param obj  chooser_info_t::obj
  /// \param n    currently selected element
  void (idaapi *enter)(void *obj, uint32 n);

  /// Callback - called from the destructor (may be NULL).
  /// \param obj  chooser_info_t::obj
  void (idaapi *destroyer)(void *obj);

  /// Callback - retrieve an element's icon (may be NULL).
  /// \param obj  chooser_info_t::obj
  /// \param n    element number
  /// \return icon id
  int (idaapi *get_icon)(void *obj, uint32 n);

  /// Callback - selection changed (may be NULL).
  /// \param obj  chooser_info_t::obj
  /// \param sel  selected elements
  void (idaapi *select)(void *obj, const intvec_t & sel);

  /// Callback - called after the chooser is finished refreshing (may be NULL).
  /// \param obj  chooser_info_t::obj
  void (idaapi *refresh)(void *obj);

  /// Callback - retrieve element attributes (may be NULL).
  /// \param obj         chooser_info_t::obj
  /// \param n           element number
  /// \param[out] attrs  result
  void (idaapi *get_attrs)(void *obj, uint32 n, chooser_item_attrs_t *attrs);

  /// Callback - for initialization.
  /// \param obj  chooser_info_t::ob
  void (idaapi *initializer)(void *obj);

  void *unused;
};

/// Multi line text control, used to embed a text control in a form
struct textctrl_info_t
{
   size_t  cb;                 ///< size of this structure
   qstring text;               ///< in, out: text control value
   uint16  flags;              ///< \ref TXTF_
/// \defgroup TXTF_ Text control property bits
/// used by textctrl_info_t::flags
//@{
#define TXTF_AUTOINDENT 0x0001 ///< auto-indent on new line
#define TXTF_ACCEPTTABS 0x0002 ///< Tab key inserts 'tabsize' spaces
#define TXTF_READONLY   0x0004 ///< text cannot be edited (but can be selected and copied)
#define TXTF_SELECTED   0x0008 ///< shows the field with its text selected
#define TXTF_MODIFIED   0x0010 ///< gets/sets the modified status
#define TXTF_FIXEDFONT  0x0020 ///< the control uses IDA's fixed font
//@}
   uint16  tabsize;            ///< how many spaces a single tab will indent
   textctrl_info_t(): cb(sizeof(textctrl_info_t)), flags(0), tabsize(0) { } ///< Constructor
};

/// \defgroup choosers Functions: generic list choosers
/// These functions display a window that allows the user to select items
//@{

/// Display a window with a list (1-column) and allow the user to select an item.
/// \param flags        \ref CH_
/// \param x0,y0,x1,y1  screen position
///                       - x0=-1 for autoposition
///                       - set x0=-2 if you need to use the chooser before opening the database
/// \param obj          object to show
/// \param width        max width of lines
/// \param sizer        number of items
/// \param getl         description of n-th item (1..n)
///                       - 0-th item if header line
///                       - 'buf' cannot be larger than MAXSTR
/// \param title        menu title (includes ptr to help). may have chooser title prefixes (see above)
/// \param icon         number of the default icon to display
/// \param deflt        item number to focus (1-based)
///                       - -1 if a non-modal chooser was already open, do not change selection
///                       - otherwise focus on the first item
/// \param del          callback for "Delete" (may be NULL)
///                       - supports multi-selection scenario too
///                       - returns: 1-ok, 0-failed
/// \param ins          callback for "New" (may be NULL)
/// \param update       callback for "Update"(may be NULL)
///                       - update the whole list
///                       - returns the new location of item 'n'
/// \param edit         callback for "Edit" (may be NULL)
/// \param enter        callback for non-modal "Enter" (may be NULL)
/// \param destroy      callback to call when the window is closed (may be NULL)
/// \param popup_names  default: insert, delete, edit, refresh
/// \param get_icon     callback for get_icon (may be NULL)
///
/// \retval -1    the chooser was already open and is now active (only for non-modal choosers)
/// \retval  0    the chooser was created but the user refused to choose anything
/// \retval else  number of them selected item

uint32 choose(
        int flags,
        int x0,int y0,
        int x1,int y1,
        void *obj,
        int width,
        uint32 (idaapi*sizer)(void *obj),
        char *(idaapi*getl)(void *obj,uint32 n,char *buf),
        const char *title,
        int icon,
        uint32 deflt=-1,
        chooser_cb_t *del=NULL,
        void (idaapi*ins)(void *obj)=NULL,
        chooser_cb_t *update=NULL,
        void (idaapi*edit)(void *obj,uint32 n)=NULL,
        void (idaapi*enter)(void * obj,uint32 n)=NULL,
        void (idaapi*destroy)(void *obj)=NULL,
        const char * const *popup_names=NULL,
        int (idaapi*get_icon)(void *obj,uint32 n)=NULL);


/// Convenience function: modal chooser.
/// see choose(), \ref CH_MODAL

inline uint32 choose(
        void *obj,
        int width,
        uint32 (idaapi*sizer)(void *obj),
        char *(idaapi*getl)(void *obj,uint32 n,char *buf),
        const char *title,
        int icon=-1,
        uint32 deflt=-1,
        chooser_cb_t *del=NULL,
        void (idaapi*ins)(void *obj)=NULL,
        chooser_cb_t *update=NULL,
        void (idaapi*edit)(void *obj,uint32 n)=NULL,
        void (idaapi*enter)(void * obj,uint32 n)=NULL,
        void (idaapi*destroy)(void *obj)=NULL,
        const char * const *popup_names=NULL,
        int (idaapi*get_icon)(void *obj,uint32 n)=NULL)
{
  return choose(CH_MODAL,-1,-1,-1,-1, obj, width, sizer,
                getl, title, icon, deflt, del, ins,
                update, edit, enter, destroy, popup_names, get_icon);
}


/// Display a generic list chooser (n-column).
/// See choose() above for the description of the undescribed parameters
/// \param ncol    number of columns
/// \param widths  widths of columns (may be NULL)
///                  - low 16 bits of each value hold the column width
///                  - high 16 bits are flags (see \ref CHCOL_)

uint32 choose2(
        int flags,
        int x0,int y0,
        int x1,int y1,
        void *obj,
        int ncol,
        const int *widths,
        uint32 (idaapi*sizer)(void *obj),
        void (idaapi*getl)(void *obj,uint32 n,char * const *arrptr),
        const char *title,
        int icon,
        uint32 deflt=-1,
        chooser_cb_t *del=NULL,
        void (idaapi*ins)(void *obj)=NULL,
        chooser_cb_t *update=NULL,
        void (idaapi*edit)(void *obj,uint32 n)=NULL,
        void (idaapi*enter)(void * obj,uint32 n)=NULL,
        void (idaapi*destroy)(void *obj)=NULL,
        const char * const *popup_names=NULL,
        int (idaapi*get_icon)(void *obj,uint32 n)=NULL);


/// Convenience function: modal chooser2.
/// see choose2(), \ref CH_MODAL

inline uint32 choose2(
        void *obj,
        int ncol,
        const int *widths,
        uint32 (idaapi*sizer)(void *),
        void (idaapi*getl)(void *,uint32,char*const*),
        const char *title,
        int icon=-1,
        uint32 deflt=-1,
        chooser_cb_t *del=NULL,
        void (idaapi*ins)(void *)=NULL,
        chooser_cb_t *update=NULL,
        void (idaapi*edit)(void *,uint32)=NULL,
        void (idaapi*enter)(void *,uint32)=NULL,
        void (idaapi*destroy)(void *)=NULL,
        const char * const *popup_names=NULL,
        int (idaapi*get_icon)(void *obj,uint32 n)=NULL)
{
  return choose2(CH_MODAL,-1,-1,-1,-1, obj, ncol, widths,
                 sizer, getl, title, icon, deflt, del,
                 ins, update, edit, enter, destroy, popup_names, get_icon);
}


/// Display a chooser with the benefit of additional callbacks.
/// see choose(), choose2()

uint32 choose3(const chooser_info_t *chi);

//@}

#endif // SWIG

#define CHOOSER_NO_SELECTION    0x01 ///< see add_chooser_command().
                                     ///< enable even if there's no selected item (n will be -1 for callback)
#define CHOOSER_MULTI_SELECTION 0x02 ///< see add_chooser_command().
                                     ///< enable for multiple selections


//-------------------------------------------------------------------------
#define CHOOSER_HOTKEY          0x08 ///< Deprecated. Please don't use
#define CHOOSER_MENU_EDIT   0        ///< Deprecated. Please don't use
#define CHOOSER_MENU_JUMP   1        ///< Deprecated. Please don't use
#define CHOOSER_MENU_SEARCH 2        ///< Deprecated. Please don't use
#define CHOOSER_POPUP_MENU  4        ///< Deprecated. Please don't use

//-------------------------------------------------------------------------

/// Navigation band colorizer function.
/// \param ea      address to calculate the color of
/// \param nbytes  number of bytes, this can be ignored for quick&dirty approach
/// \return color of the specified address in RGB

typedef uint32 idaapi nav_colorizer_t(ea_t ea, asize_t nbytes);


/// Install new navigation band colorizer (::ui_set_nav_colorizer).
/// \return the previous colorizer function

inline nav_colorizer_t *set_nav_colorizer(nav_colorizer_t *func)
{
  return (nav_colorizer_t *)(callui(ui_set_nav_colorizer, func).vptr);
}

namespace Controls
{
  class TWinControl;
  class TCustomControl; // ptr to custom ida viewer
}
using Controls::TWinControl;
using Controls::TCustomControl;

/// Custom viewer & code viewer handler types
enum custom_viewer_handler_id_t
{
  CVH_USERDATA,
  CVH_KEYDOWN,               ///< see ::custom_viewer_keydown_t
  CVH_POPUP,                 ///< see ::custom_viewer_popup_t
  CVH_DBLCLICK,              ///< see ::custom_viewer_dblclick_t
  CVH_CURPOS,                ///< see ::custom_viewer_curpos_t
  CVH_CLOSE,                 ///< see ::custom_viewer_close_t
  CVH_CLICK,                 ///< see ::custom_viewer_click_t
  CVH_QT_AWARE,              ///< see set_custom_viewer_qt_aware()
  CVH_HELP,                  ///< see ::custom_viewer_help_t
  CVH_MOUSEMOVE,             ///< see ::custom_viewer_mouse_moved_t

  CDVH_USERDATA = 1000,      ///< see set_code_viewer_user_data()
  CDVH_SRCVIEW,              ///< see set_code_viewer_is_source()
  CDVH_LINES_CLICK,          ///< see ::code_viewer_lines_click_t
  CDVH_LINES_DBLCLICK,       ///< see ::code_viewer_lines_click_t
  CDVH_LINES_POPUP,          ///< see ::code_viewer_lines_click_t
  CDVH_LINES_DRAWICON,       ///< see ::code_viewer_lines_icon_t
  CDVH_LINES_LINENUM,        ///< see ::code_viewer_lines_linenum_t
  CDVH_LINES_ICONMARGIN,     ///< see set_code_viewer_lines_icon_margin()
  CDVH_LINES_RADIX,          ///< see set_code_viewer_lines_radix()
  CDVH_LINES_ALIGNMENT       ///< see set_code_viewer_lines_alignment()
};

//-------------------------------------------------------------------------
/// state & 1 => Shift is pressed                 \n
/// state & 2 => Alt is pressed                   \n
/// state & 4 => Ctrl is pressed                  \n
/// state & 8 => Mouse left button is pressed     \n
/// state & 16 => Mouse right button is pressed   \n
/// state & 32 => Mouse middle button is pressed  \n
/// state & 128 => Meta is pressed (OSX only)
typedef int view_event_state_t;

//-------------------------------------------------------------------------
/// Notification codes for events in the message window
enum msg_notification_t
{
  msg_activated,    ///< The message window is activated.
                    ///< \param none
                    ///< \return void

  msg_deactivated,  ///< The message window is deactivated.
                    ///< \param none
                    ///< \return void

  obsolete_msg_popup,

  msg_click,        ///< Click event.
                    ///< \param x      (int) x-coordinate
                    ///< \param y      (int) y-coordinate
                    ///< \param state  (::view_event_state_t)
                    ///< \retval 1 handled
                    ///< \retval 0 not handled (invoke default handler)

  msg_dblclick,     ///< Double click event.
                    ///< \param x      (int) x-coordinate
                    ///< \param y      (int) y-coordinate
                    ///< \param state  (::view_event_state_t)
                    ///< \retval 1 handled
                    ///< \retval 0 not handled (invoke default handler)

  msg_closed,       ///< View closed.
                    ///< \param none
                    ///< \return void

  msg_keydown,      ///< Key down event.
                    ///< \param key    (int)
                    ///< \param state  (::view_event_state_t)
                    ///< \retval 1 handled
                    ///< \retval 0 not handled (invoke default handler)
};

//-------------------------------------------------------------------------
/// Information about a position relative to the renderer
struct renderer_pos_info_t
{
  /// Constructor
  renderer_pos_info_t() : node(-1), cx(-1), cy(-1), sx(-1) {}

  int node; ///< the node, or -1 if the current renderer
            ///< is not a graph renderer.

  short cx; ///< the X coords of the character in the current line.
            ///< When in graph mode: X coords of the character in 'node'.       \n
            ///< When in flat mode: X coords of the character in the line, w/o  \n
            ///< taking scrolling into consideration.

  short cy; ///< the Y coords of the character.
            ///< When in graph mode: Y coords of the character in 'node'.       \n
            ///< When in flat mode: Line number, starting from the top.

  short sx; ///< the number of chars that are scrolled (flat mode only)
};

//-------------------------------------------------------------------------
#ifndef SWIG
struct selection_item_t;

/// Information about a mouse action within a view
struct view_mouse_event_t
{
  tcc_renderer_type_t rtype;        ///< type of renderer that received the event

  uint32 x;                         ///< screen x coordinate
  uint32 y;                         ///< screen y coordinate

  /// Abstraction of location in flat view/graph views
  union location_t
  {
    ea_t ea;                        ///< flat view (rtype == ::TCCRT_FLAT)
    const selection_item_t *item;   ///< graph views (rtype != ::TCCRT_FLAT).
                                    ///< NULL if mouse is not currently over an item.
  };
  location_t location;              ///< location where event was generated

  view_event_state_t state;         ///< contains information about what buttons are CURRENTLY pressed
                                    ///< on the keyboard and mouse. view_mouse_event_t instances created
                                    ///< in functions like mouseReleaseEvent() won't contain any information
                                    ///< about the mouse, because it has been released.

  vme_button_t button;              ///< represents which mouse button was responsible for generating the event.
                                    ///< This field does not care about the current state of the mouse.

  renderer_pos_info_t renderer_pos; ///< position where event was generated, relative to the renderer
};
#endif

//-------------------------------------------------------------------------
/// Notification codes sent by the UI for IDAView or custom viewer events.
/// These notification codes should be used together with ::HT_VIEW hook type.
enum view_notification_t
{
  view_activated,    ///< A view is activated
                     ///< \param view  (TCustomControl *)

  view_deactivated,  ///< A view is deactivated
                     ///< \param view  (TCustomControl *)

  view_keydown,      ///< Key down event
                     ///< \param view   (TCustomControl *)
                     ///< \param key    (int)
                     ///< \param state  (::view_event_state_t)

  obsolete_view_popup,

  view_click,        ///< Click event
                     ///< \param view   (TCustomControl *)
                     ///< \param event  (const ::view_mouse_event_t *)

  view_dblclick,     ///< Double click event
                     ///< \param view   (TCustomControl *)
                     ///< \param event  (const ::view_mouse_event_t *)

  view_curpos,       ///< Cursor position changed
                     ///< \param view  (TCustomControl *)

  view_created,      ///< A view is being created.
                     ///< \param view  (TCustomControl *)

  view_close,        ///< View closed
                     ///< \param view  (TCustomControl *)

  view_switched,     ///< A view's renderer has changed.
                     ///< \param view  (TCustomControl *)
                     ///< \param rt    (::tcc_renderer_type_t)

  view_mouse_over,   ///< The user moved the mouse over (or out of) a node or an edge.
                     ///< This is only relevant in a graph view.
                     ///< \param view   (TCustomControl *)
                     ///< \param event  (const ::view_mouse_event_t *)
};


/// The user has pressed a key

typedef bool idaapi custom_viewer_keydown_t(TCustomControl *cv, int vk_key, int shift, void *ud);


/// The user right clicked. See ::ui_populating_tform_popup, too.

typedef void idaapi custom_viewer_popup_t(TCustomControl *cv, void *ud);


/// The user moved the mouse.

typedef void idaapi custom_viewer_mouse_moved_t(TCustomControl *cv, int shift, view_mouse_event_t *e, void *ud);


/// The user clicked

typedef bool idaapi custom_viewer_click_t(TCustomControl *cv, int shift, void *ud);


/// The user double clicked

typedef bool idaapi custom_viewer_dblclick_t(TCustomControl *cv, int shift, void *ud);


/// Cursor position has been changed

typedef void idaapi custom_viewer_curpos_t(TCustomControl *cv, void *ud);


/// Custom viewer is being destroyed

typedef void idaapi custom_viewer_close_t(TCustomControl *cv, void *ud);


/// Custom viewer: the user pressed F1
/// If the return value != -1, it is treated as a help context to display (from ida.hlp)

typedef int idaapi custom_viewer_help_t(TCustomControl *cv, void *ud);

// Code viewer handlers for the lineinfo widget located to the left of the text.

/// The user clicked, right clicked or double clicked.
/// pos: the clicked icon number. -1 means the click occurred on space not reserved to icons.

typedef void idaapi code_viewer_lines_click_t(TCustomControl *c, const place_t *p, int pos, int shift, void *ud);


/// Icon drawing.
/// \param pos  the icon number, will be 0,1,2,3...                                  \n
///             can be modified to skip positions and draw at the specified one
/// \return the id of the icon to draw. If bitwise or'ed with 0x80000000,
///         IDA calls this function once more with pos+1 to retrieve one more icon.

typedef int idaapi code_viewer_lines_icon_t(TCustomControl *cv, const place_t *p, int *pos, void *ud);


/// Calculate the line number. Return false to not print any number.

typedef bool idaapi code_viewer_lines_linenum_t(TCustomControl *cv, const place_t *p, uval_t *num, void *ud);


//------------------------------------------------------------------------

/// Command line interpreter.
/// Provides functionality for the command line (located at the bottom of the main window).
/// Only GUI version of IDA supports CLIs.
struct cli_t
{
  size_t size;                  ///< size of this structure
  int32 flags;                  ///< \ref CLIF_
/// \defgroup CLIF_ CLI attributes
/// used by cli_t::flags
//@{
#define CLIF_QT_AWARE    1      ///< keydown event will use Qt key codes
//@}
  const char *sname;            ///< short name (displayed on the button)
  const char *lname;            ///< long name (displayed in the menu)
  const char *hint;             ///< hint for the input line

  /// Callback: the user pressed Enter.
  /// CLI is free to execute the line immediately or ask for more lines.
  /// \param  line   command to execute
  /// \retval true   executed line
  /// \retval false  ask for more lines
  bool (idaapi *execute_line)(const char *line);

  /// Callback: the user pressed Tab.
  /// This callback is optional.
  /// \param[out] completion  result of completion
  /// \param prefix           text to complete
  /// \param n                completion number
  /// \param line             entire command line, given as context information
  /// \param x                index where 'prefix' starts in 'line'
  /// \retval true            generated a new completion
  /// \retval false           otherwise
  bool (idaapi *complete_line)(
        qstring *completion,
        const char *prefix,
        int n,
        const char *line,
        int x);

  /// Callback: a keyboard key has been pressed.
  /// This callback is optional.
  /// It is a generic callback and the CLI is free to do whatever it wants.
  /// \param line      current input line (in/out argument)
  /// \param p_x       pointer to current x coordinate of the cursor (in/out)
  /// \param p_sellen  pointer to current selection length (usually 0)
  /// \param p_vk_key  pointer to virtual key code (in/out).
  ///                   if the key has been handled, it should be reset to 0 by CLI
  /// \param shift     shift state
  /// \retval true modified input line or x coordinate or selection length
  /// \retval false otherwise
  bool (idaapi *keydown)(
        qstring *line,
        int *p_x,
        int *p_sellen,
        int *vk_key,
        int shift);
};

//---------------------------------------------------------------------------
/// \defgroup MFF_ Exec request flags
/// passed as 'reqf' parameter to execute_sync()
//@{
#define MFF_FAST   0x0000       ///< Execute code as soon as possible.
                                ///< this mode is ok for calling ui related functions
                                ///< that do not query the database.

#define MFF_READ   0x0001       ///< Execute code only when ida is idle and it is safe
                                ///< to query the database.
                                ///< this mode is recommended only
                                ///< for code that does not modify the database.
                                ///< (nb: ida may be in the middle of executing
                                ///< another user request, for example it may be waiting
                                ///< for him to enter values into a modal dialog box)

#define MFF_WRITE  0x0002       ///< Execute code only when ida is idle and it is safe
                                ///< to modify the database. in particular,
                                ///< this flag will suspend execution if there is
                                ///< a modal dialog box on the screen
                                ///< this mode can be used to call any ida api function
                                ///< #MFF_WRITE implies #MFF_READ

#define MFF_NOWAIT 0x0004       ///< Do not wait for the request to be executed.
                                ///< the caller should ensure that the request is not
                                ///< destroyed until the execution completes.
                                ///< if not, the request will be ignored.
                                ///< execute_sync() returns the request id in this case.
                                ///< it can be used in cancel_exec_request().
                                ///< This flag can be used to delay the code execution
                                ///< until the next UI loop run even from the main thread.
//@}


/// Execute code in the main thread - to be used with execute_sync().
struct exec_request_t
{
  /// Internal magic
  enum { MFF_MAGIC = 0x12345678 };

  /// Can this request be executed?
  bool valid(void) const
  {
    return (code & ~7) == MFF_MAGIC && (sem != NULL || (code & MFF_NOWAIT) != 0);
  }

  int code;           ///< temporary location, used internally

  qsemaphore_t sem;   ///< semaphore to communicate with the main thread.
                      ///< If NULL, will be initialized by execute_sync().

  /// Callback to be executed.
  /// If this function raises an exception, execute_sync() never returns.
  virtual int idaapi execute(void) = 0;

  /// Constructor
  exec_request_t(void) : sem(NULL) {}

  /// Destructor
  // FIXME: windows: gcc compiled plugins can not use exec_request_t because the destructor
  // is generated differently!
  virtual ~exec_request_t(void) { qsem_free(sem); sem = NULL; code = 0; }
};

//---------------------------------------------------------------------------
/// Base class for defining UI requests.
/// Override the run() method and insert your code.
class ui_request_t
{
public:
  /// Run the UI request
  /// \retval false  remove the request from the queue
  /// \retval true   reschedule the request and run it again
  virtual bool idaapi run() = 0;
  DEFINE_VIRTUAL_DTOR(ui_request_t);
};

/// List of UI requests. The ui_request_t is allocated by the caller
/// but its ownership is transferred to the execute_ui_requests().
/// The ui_request_t instance will be deleted as soon as it is executed and
/// was not rescheduled for another run.
class ui_requests_t : public qlist<ui_request_t *>
{
  DECLARE_UNCOPYABLE(ui_requests_t)
public:
  ui_requests_t() {}  ///< Constructor
  ~ui_requests_t()    ///< Destructor
  {
    for ( iterator p=begin(); p != end(); ++p )
      delete *p;
  }
};

/// Snapshot restoration completion callback. see restore_database_snapshot()
typedef void (idaapi *ss_restore_cb_t)(const char *errmsg, void *ud);

/// Fill loc with the current location data
typedef void idaapi fillloc_t(location_t *loc, void *ud);
/// Jump to the specified location
typedef void idaapi jumploc_t(location_t *loc, int uijmp_flags, void *ud);

/// \defgroup UIJMP_ Jump flags
/// passed as 'uijmp_flags' parameter to jumpto()
//@{
#define UIJMP_ACTIVATE 0x0001  ///< activate the new window
#define UIJMP_DONTPUSH 0x0002  ///< do not remember the current address
                               ///< in the navigation history
#define UIJMP_IDAVIEW  0x0004  ///< jump in idaview (by default any eaview is good)
//@}

//-------------------------------------------------------------------------
#ifndef __UI__
namespace Forms
{
  class TForm;
}
using Forms::TForm;
#endif // __UI__

//-------------------------------------------------------------------------
/// Maintain information about the current state of the UI.
/// This allows actions to behave appropriately (see ::action_handler_t)
struct action_ctx_base_t
{
  /// Constructor
  action_ctx_base_t()
  {
    cur_sel.from.at = NULL;
    cur_sel.to.at = NULL;
    reset();
  }

  /// Invalidate all context info
  void reset()
  {
    form = NULL;
    form_type = BWN_UNKNOWN;
    form_title.clear();
    chooser_selection.clear();
    action = NULL;

    //
    cur_flags = 0;
    cur_ea = cur_extracted_ea = BADADDR;
    cur_func = cur_fchunk = NULL;
    cur_struc = NULL; cur_strmem = NULL;
    cur_enum = enum_t(-1);
    cur_seg = NULL;

    cur_sel.from.at = NULL;
    cur_sel.from.x = -1;
    cur_sel.to.at = NULL;
    cur_sel.to.x = -1;
  }
#ifndef __UI__
  TForm *form;
#else
  void *form;
#endif // __UI__

  tform_type_t form_type;         ///< type of current form
  qstring form_title;             ///< title of current form
  intvec_t chooser_selection;     ///< current chooser selection
  const char *action;             ///< action name

  //-------------------------------------------------------------------------
  uint32 cur_flags; ///< Current address information. see \ref ACF_
/// \defgroup ACF_ Action context property bits
/// used by action_ctx_base_t::cur_flags
//@{
#define ACF_HAS_SELECTION 1 << 0 ///< there is currently a valid selection
#define ACF_XTRN_EA       1 << 1 ///< cur_ea is in 'externs' segment
//@}

  /// Check if the given flag is set
  inline bool has_flag(uint32 flag) const { return (cur_flags & flag) == flag; }

  ea_t cur_ea;           ///< the current EA of the position in the view
  ea_t cur_extracted_ea; ///< the possible EA the cursor is positioned on

  func_t *cur_func;      ///< the current function
  func_t *cur_fchunk;    ///< the current function chunk

  struc_t *cur_struc;    ///< the current structure
  member_t *cur_strmem;  ///< the current structure member

  enum_t cur_enum;       ///< the current enum

  segment_t *cur_seg;    ///< the current segment

  struct
  {
    twinpos_t from;      ///< start of selection
    twinpos_t to;        ///< end of selection
  } cur_sel;             ///< the currently selected area. also see #ACF_HAS_SELECTION
};

//-------------------------------------------------------------------------
/// Instances of this class will be filled with information that is
/// commonly used by actions when they need to
/// be activated. This is so they don't have to perform (possibly)
/// costly operations more than once.
struct action_activation_ctx_t : public action_ctx_base_t
{
};

//-------------------------------------------------------------------------
/// Instances of this class will be filled with information that is
/// commonly used by actions when they need to
/// update. This is so they don't have to perform (possibly)
/// costly operations more than once.
struct action_update_ctx_t : public action_ctx_base_t
{
};

#define AHF_VERSION 1          ///< action handler version (used by action_handler_t::flags)
#define AHF_VERSION_MASK 0xFF  ///< mask for action_handler_t::flags

//-------------------------------------------------------------------------
/// Action states - returned by action_handler_t::update()
enum action_state_t
{
  AST_ENABLE_ALWAYS,    ///< enable action and do not call action_handler_t::update() anymore

  AST_ENABLE_FOR_IDB,   ///< enable action for the current idb.
                        ///< call action_handler_t::update() when a database is opened/closed

  AST_ENABLE_FOR_FORM,  ///< enable action for the current form.
                        ///< call action_handler_t::update() when a form gets/loses focus

  AST_ENABLE,           ///< enable action - call action_handler_t::update() when anything changes

  AST_DISABLE_ALWAYS,   ///< disable action and do not call action_handler_t::action() anymore
  AST_DISABLE_FOR_IDB,  ///< analog of ::AST_ENABLE_FOR_IDB
  AST_DISABLE_FOR_FORM, ///< analog of ::AST_ENABLE_FOR_FORM
  AST_DISABLE,          ///< analog of ::AST_ENABLE
};


/// Check if the given action state is one of AST_ENABLE*

inline bool is_action_enabled(action_state_t s)
{
  return s <= AST_ENABLE;
}

//-------------------------------------------------------------------------
/// Manages the behavior of a registered action
struct action_handler_t
{
  int flags;  ///< internal - for version management

  /// Constructor
  action_handler_t(int _f = 0) : flags(_f) { flags |= AHF_VERSION; }

  /// Activate an action.
  /// This function implements the core behavior of an action.
  /// It is called when the action is triggered, from a menu, from
  /// a popup menu, from the toolbar, or programmatically.
  /// \returns non-zero: all IDA windows will be refreshed
  virtual int idaapi activate(action_activation_ctx_t *ctx) = 0;

  /// Update an action.
  /// This is called when the context of the UI changed, and we need to let the
  /// action update some of its properties if needed (label, icon, ...)
  ///
  /// In addition, this lets IDA know whether the action is enabled,
  /// and when it should be queried for availability again.
  ///
  /// Note: This callback is not meant to change anything in the
  /// application's state, except by calling one (or many) of
  /// the "update_action_*()" functions on this very action.
  virtual action_state_t idaapi update(action_update_ctx_t *ctx) = 0;

  /// Destructor
  virtual idaapi ~action_handler_t() {}
};

/// Describe an action to be registered (see register_action())
struct action_desc_t
{
  size_t cb;                 ///< size of this structure
  const char *name;          ///< the internal name of the action; must be unique.
                             ///< a way to reduce possible conflicts is to prefix it
                             ///< with some specific prefix. E.g., "myplugin:doSthg".

  const char *label;         ///< the label of the action, possibly with hotkey
                             ///< definition (e.g., "~J~ump to operand")

  action_handler_t *handler; ///< the action handler, for activating/updating.
                             ///< please read the comments at register_action().

  const plugin_t *owner;     ///< plugin responsible for registering action, can be NULL

  const char *shortcut;      ///< an optional shortcut definition. E.g., "Ctrl+Enter"
  const char *tooltip;       ///< an optional tooltip for the action
  int icon;                  ///< an optional icon ID to use
};

/// Get an ::action_desc_t instance with your plugin as the owner
#define ACTION_DESC_LITERAL(name, label, handler, shortcut, tooltip, icon)\
  { sizeof(action_desc_t), name, label, handler, &PLUGIN, shortcut, tooltip, icon }

/// Get an ::action_desc_t instance with a given owner
#define ACTION_DESC_LITERAL_OWNER(name, label, handler, owner, shortcut, tooltip, icon) \
  { sizeof(action_desc_t), name, label, handler, owner, shortcut, tooltip, icon }

/// For attach_dynamic_action_to_popup() only
#define DYNACTION_DESC_LITERAL(label, handler, shortcut, tooltip, icon) \
  ACTION_DESC_LITERAL_OWNER(NULL, label, handler, NULL, shortcut, tooltip, icon)

/// Codes for getting/setting action attributes
enum action_attr_t
{
  AA_NONE,        ///< no effect
  AA_LABEL,       ///< see update_action_label()
  AA_SHORTCUT,    ///< see update_action_shortcut()
  AA_TOOLTIP,     ///< see update_action_tooltip()
  AA_ICON,        ///< see update_action_icon()
  AA_STATE,       ///< see update_action_state()
  AA_CHECKABLE,   ///< see update_action_checkable()
  AA_CHECKED,     ///< see update_action_checked()
  AA_VISIBILITY,  ///< see update_action_visibility()
};

//-------------------------------------------------------------------------
/// API bw-compat only. You shouldn't be using this, as it will
/// be deprecated soon. This is only to ease transition for
/// older plugins/processor modules.
typedef bool idaapi menu_item_callback_t(void *ud);

/// Specify that an action belongs to the current processor module.
/// see action_desc_t::owner and #ACTION_DESC_LITERAL_OWNER
#define CURPROC_ACTION_OWNER ((const plugin_t *) 1)

#ifndef __UI__         // Not for the UI

// Convenience functions offered by the user interface

/// Execute a list of UI requests (::ui_execute_ui_requests_list).
/// \returns a request id: a unique number that can be used to cancel the request

THREAD_SAFE inline int execute_ui_requests(ui_requests_t *reqs)
{
  return callui(ui_execute_ui_requests_list, reqs).i;
}


/// Execute a variable number of UI requests (::ui_execute_ui_requests).
/// The UI requests will be dispatched in the context of the main thread.
/// \param req  pointer to the first request ,use NULL to terminate the var arg request list
/// \return a request id: a unique number that can be used to cancel the request

THREAD_SAFE inline int execute_ui_requests(ui_request_t *req, ...)
{
  va_list va;
  va_start(va, req);
  int req_id = callui(ui_execute_ui_requests, req, va).i;
  va_end(va);
  return req_id;
}


/// Try to cancel an asynchronous exec request (::ui_cancel_exec_request).
/// \param req_id  request id
/// \retval true   successfully canceled
/// \retval false  request has already been processed.

THREAD_SAFE inline bool cancel_exec_request(int req_id)
{
  return callui(ui_cancel_exec_request, req_id).cnd;
}


/// Jump to the specified address (::ui_jumpto).
/// \param ea           destination
/// \param opnum        -1: don't change x coord
/// \param uijmp_flags  \ref UIJMP_
/// \return success

inline bool jumpto(ea_t ea, int opnum=-1, int uijmp_flags=UIJMP_ACTIVATE)
{
  return callui(ui_jumpto, ea, opnum, uijmp_flags).cnd;
}


/// Show a banner dialog box (::ui_banner).
/// \param wait  time to wait before closing
/// \retval 1    ok
/// \retval 0    esc was pressed

inline bool banner(int wait)               { return callui(ui_banner, wait).cnd;}


/// Can we use msg() functions?

THREAD_SAFE inline bool is_msg_inited(void){ return callui(ui_is_msg_inited).cnd; }


/// Refresh marked windows (::ui_refreshmarked)

inline void refresh_idaview(void)          { callui(ui_refreshmarked);}


/// Refresh all disassembly views (::ui_refresh), forces an immediate refresh.
/// Please consider request_refresh() instead

inline void refresh_idaview_anyway(void)   { callui(ui_refresh);      }


/// Allow the user to set analyzer options. (show a dialog box) (::ui_analyzer_options)

inline void analyzer_options(void)         { callui(ui_analyzer_options); }


/// Get the address at the screen cursor (::ui_screenea)

inline ea_t get_screen_ea(void)            { ea_t ea; callui(ui_screenea, &ea); return ea; }


/// Get current operand number, -1 means no operand (::ui_get_opnum)

inline int get_opnum(void)                 { return callui(ui_get_opnum).i; }


/// Get the cursor position on the screen (::ui_get_cursor).
/// \note coordinates are 0-based
/// \param[out] x  x-coordinate
/// \param[out] y  y-coordinate
/// \retval true   pointers are filled
/// \retval false  no disassembly window open

inline bool get_cursor(int *x, int *y)     { return callui(ui_get_cursor, x, y).cnd; }


/// Get coordinates of the output window's cursor (::ui_get_output_cursor).
/// \note coordinates are 0-based
/// \note this function will succeed even if the output window is not visible
/// \param[out] x   column
/// \param[out] y   line number (global, from the start of output)
/// \retval false   the output window has been destroyed.
/// \retval true    pointers are filled

inline bool get_output_cursor(int *x, int *y) { return callui(ui_get_output_cursor, x, y).cnd; }


/// Get current line from the disassemble window (::ui_get_curline).
/// \return cptr  current line with the color codes
/// (use tag_remove() to remove the color codes)

inline char *get_curline(void)             { return callui(ui_get_curline).cptr; }


/// Get the selected area boundaries (::ui_readsel). See also: readsel2()
/// \param[out] ea1 start ea
/// \param[out] ea2 end ea
/// \retval 0 no area is selected \n
/// \retval 1 ok, start ea and end ea are filled

inline bool read_selection(ea_t *ea1, ea_t *ea2) { return callui(ui_readsel, ea1, ea2).cnd; }


/// Unmark selection (::ui_unmarksel)

inline void unmark_selection(void)         { callui(ui_unmarksel); }


/// Open the given url (::ui_open_url)

inline void open_url(const char *url)      { callui(ui_open_url, url); }


/// Get the current address in a hex view.
/// \param hexdump_num number of hexview window

inline ea_t get_hexdump_ea(int hexdump_num) { ea_t ea; callui(ui_hexdumpea, &ea, hexdump_num); return ea; }


/// Get keyboard key code by its name (::ui_get_key_code)

inline ushort get_key_code(const char *keyname) { return callui(ui_get_key_code, keyname).i16; }


/// Get shortcut code previously created by ::ui_get_key_code.
/// \param key    key constant
/// \param shift  modifiers
/// \param is_qt  are we using gui version?

inline ushort lookup_key_code(int key, int shift, bool is_qt) { return callui(ui_lookup_key_code, key, shift, is_qt).i16; }


/// Refresh navigation band if changed (::ui_refresh_navband).
/// \param force refresh regardless

inline void refresh_navband(bool force)     { callui(ui_refresh_navband, force); }


/// Mark a non-modal custom chooser for a refresh (::ui_refresh_chooser).
/// \param title  title of chooser
/// \return success

inline bool refresh_chooser(const char *title) { return callui(ui_refresh_chooser, title).cnd; }


/// Close a non-modal chooser (::ui_close_chooser).
/// \param title window title of chooser to close
/// \return success

inline bool close_chooser(const char *title) { return callui(ui_close_chooser, title).cnd; }


/// Install command line interpreter (::ui_install_cli)

inline void install_command_interpreter(const cli_t *cp) { callui(ui_install_cli, cp, true); }


/// Remove command line interpreter (::ui_install_cli)

inline void remove_command_interpreter(const cli_t *cp) { callui(ui_install_cli, cp, false); }


/// Generate disassembly text for a range.
/// \param ea1        start address
/// \param ea2        end address
/// \param[out] text  result
/// \param truncate_lines  (on idainfo::margin)

inline void gen_disasm_text(ea_t ea1, ea_t ea2, text_t &text, bool truncate_lines) { callui(ui_gen_disasm_text, ea1, ea2, &text, truncate_lines); }


/// Execute code in the main thread.
/// \param req   request specifying the code to execute
/// \param reqf  \ref MFF_
/// \return if \ref #MFF_NOWAIT is specified, return the request id.
///         otherwise return the value returned by exec_request_t::execute().

THREAD_SAFE inline int execute_sync(exec_request_t &req, int reqf) { return callui(ui_execute_sync, &req, reqf).i; }


/// Set the docking position of a form (::ui_set_dock_pos).
/// \param src_ctrl                title of form to dock
/// \param dest_ctrl               where to dock: if NULL or invalid then create
///                                a new tab relative to current active tab
/// \param orient                  \ref DP_
/// \param left,top,right,bottom   dimensions of dock, if not specified or invalid then
///                                create the form in the center of the screen with the
///                                default size
/// \return success

inline bool set_dock_pos(const char *src_ctrl, const char *dest_ctrl, int orient, int left = 0, int top = 0, int right = 0, int bottom = 0)
{
  return callui(ui_set_dock_pos, src_ctrl, dest_ctrl, orient, left, top, right, bottom).cnd;
}


/// Load an icon from a file (::ui_load_custom_icon_file).
/// Also see load_custom_icon(const void *, unsigned int, const char *)
/// \param file_name path  to file
/// \return icon id

inline int load_custom_icon(const char *file_name) { return callui(ui_load_custom_icon_file, file_name).i; }


/// Load an icon and return its id (::ui_load_custom_icon).
/// \param ptr     pointer to raw image data
/// \param len     image data length
/// \param format  image format
/// \return icon id

inline int load_custom_icon(const void *ptr, unsigned int len, const char *format) { return callui(ui_load_custom_icon, ptr, len, format).i; }


/// Free an icon loaded with load_custom_icon() (::ui_free_custom_icon).

inline void free_custom_icon(int icon_id) { callui(ui_free_custom_icon, icon_id); }


/// Processes a UI action by name.
/// \param name   action name
/// \param flags  reserved/not used
/// \param param  reserved/not used

inline bool process_ui_action(const char *name, int flags=0, void *param=NULL)
{
  return callui(ui_process_action, name, flags, param).cnd;
}


/// Take a database snapshot (::ui_take_database_snapshot).
/// \param ss       in/out parameter.
///                   - in: description, flags
///                   - out: filename, id
/// \param err_msg  optional error msg buffer
/// \return success

inline bool take_database_snapshot(
      snapshot_t *ss,
      qstring *err_msg)
{
  return callui(ui_take_database_snapshot, ss, err_msg).cnd;
}


/// Restore a database snapshot.
/// Note: This call is asynchronous. When it is completed, the callback will be triggered.
/// \param ss  snapshot instance (see build_snapshot_tree())
/// \param cb  A callback that will be triggered with a NULL string.
///             on success and an actual error message on failure.
/// \param ud  user data passed to be passed to the callback
/// \return false if restoration could not be started (snapshot file was not found).  \n
///         If the returned value is True then check if the operation succeeded from the callback.

inline bool restore_database_snapshot(
      const snapshot_t *ss,
      ss_restore_cb_t cb,
      void *ud)
{
  return callui(ui_restore_database_snapshot, ss, cb, ud).cnd;
}

/// Timer opaque handle
typedef struct __qtimer_t {} *qtimer_t;


/// Register a timer (::ui_register_timer).
/// Timer functions are thread-safe and the callback is executed
/// in the context of the main thread.
/// \param interval_ms  interval in milliseconds
/// \param callback     the callback can return -1 to unregister the timer;
///                     any other value >= 0 defines the new interval for the timer
/// \param ud callback  params
/// \return handle to registered timer (use this handle to unregister it)

THREAD_SAFE inline qtimer_t register_timer(
        int interval_ms,
        int (idaapi *callback)(void *ud),
        void *ud)
{
  return (qtimer_t)(callui(ui_register_timer, interval_ms, callback, ud).vptr);
}


/// Unregister a timer (::ui_unregister_timer).
/// \param t handle to a registered timer
/// \return success

THREAD_SAFE inline bool unregister_timer(qtimer_t t)
{
  return callui(ui_unregister_timer, t).cnd;
}

//-------------------------------------------------------------------------

/// Create a new action (::ui_register_action).
/// After an action has been created, it is possible to attach it
/// to menu items (attach_action_to_menu()), or to popup menus
/// (attach_action_to_popup()).
///
/// Because the actions will need to call the handler's activate() and
/// update() methods at any time, you shouldn't build your action handler
/// on the stack.
///
/// Please see the SDK's "ht_view" plugin for an example how
/// to register actions.
/// \param desc action to register
/// \return success

inline bool register_action(const action_desc_t &desc)
{
  return callui(ui_register_action, &desc).cnd;
}


/// Delete a previously-registered action (::ui_unregister_action).
/// \param name  name of action
/// \return success

inline bool unregister_action(const char *name)
{
  return callui(ui_unregister_action, name).cnd;
}


/// Attach a previously-registered action to the menu (::ui_attach_action_to_menu).
/// \note You should not change top level menu, or the Edit,Plugins submenus
/// If you want to modify the debugger menu, do it at the ui_debugger_menu_change
/// event (ida might destroy your menu item if you do it elsewhere).
/// \param menupath  path to the menu item after or before which the insertion will take place.  \n
///                    - Example: Debug/StartProcess
///                    - Whitespace, punctuation are ignored.
///                    - It is allowed to specify only the prefix of the menu item.
///                    - Comparison is case insensitive.
///                    - menupath may start with the following prefixes:
///                    - [S] - modify the main menu of the structure window
///                    - [E] - modify the main menu of the enum window
/// \param name      the action name
/// \param flags     a combination of \ref SETMENU_, to determine menu item position
/// \return success

inline bool attach_action_to_menu(
        const char *menupath,
        const char *name,
        int flags)
{
  return callui(ui_attach_action_to_menu, menupath, name, flags).cnd;
}


/// Detach an action from the menu (::ui_detach_action_from_menu).
/// \param menupath   path to the menu item
/// \param name       the action name
/// \return success

inline bool detach_action_from_menu(
        const char *menupath,
        const char *name)
{
  return callui(ui_detach_action_from_menu, menupath, name).cnd;
}


/// Attach an action to an existing toolbar (::ui_attach_action_to_toolbar).
/// \param toolbar_name  the name of the toolbar
/// \param name          the action name
/// \return success

inline bool attach_action_to_toolbar(
        const char *toolbar_name,
        const char *name)
{
  return callui(ui_attach_action_to_toolbar, toolbar_name, name).cnd;
}


/// Detach an action from the toolbar (::ui_detach_action_from_toolbar).
/// \param toolbar_name  the name of the toolbar
/// \param name          the action name
/// \return success

inline bool detach_action_from_toolbar(
  const char *toolbar_name,
  const char *name)
{
  return callui(ui_detach_action_from_toolbar, toolbar_name, name).cnd;
}


/// Helper.
///
/// You are not encouraged to use this, as it mixes flags for
/// both register_action(), and attach_action_to_menu().
///
/// The only reason for its existence is to make it simpler
/// to port existing plugins to the new actions API.

inline bool register_and_attach_to_menu(
        const char *menupath,
        const char *name,
        const char *label,
        const char *shortcut,
        int flags,
        action_handler_t *handler,
        const plugin_t *owner)
{
  action_desc_t desc = ACTION_DESC_LITERAL_OWNER(name, label, handler, owner, shortcut, NULL, -1);
  if ( !register_action(desc) )
    return false;
  if ( !attach_action_to_menu(menupath, name, (flags & SETMENU_POSMASK)) )
  {
    unregister_action(name);
    return false;
  }
  return true;
}


/// Set the icon of a menu item (::ui_set_menu_item_icon).
/// \return success

inline bool set_menu_item_icon(const char *item_name, int icon_id)
{ return callui(ui_set_menu_item_icon, item_name, icon_id).cnd; }


/// Enable or disable a menu item (::ui_enable_menu_item).
/// \return success

inline bool enable_menu_item(const char *item_name, bool enable)
{ return callui(ui_enable_menu_item, item_name, enable).cnd; }

//------------------------------------------------------------------------
// Get VCL global variables

namespace Menus
{
  class TPopupMenu;
}
using Menus::TPopupMenu;

#ifndef _WINDOWS_
typedef struct HWND__ *HWND;
#endif


/// Create a new tform (only gui version) (::ui_create_tform).
/// Hook to ::ui_tform_visible event instead.
/// \param caption      window caption
/// \param[out] handle  handle of resulting window.
///                      you will need this if you do not use VCL.
/// \returns
/// If a window with the specified caption exists, return
/// a pointer to it. 'handle' will be NULL is this case.        \n
/// NB: Do not use 'handle' to populate the window,
/// because it can be destroyed by the user interface
/// at any time.                                                \n
/// Also, the handle is invalid at the form creation time.
/// It is present only for compatibility reasons.               \n
/// The text version always returns NULL.

inline TForm *create_tform(const char *caption, HWND *handle)
{
  return (TForm *)callui(ui_create_tform, caption, handle).vptr;
}


/// Open a TForm (only gui version) (::ui_open_tform).
/// \param form     form to display
/// \param options  \ref FORM_OPEN

inline void open_tform(TForm *form, int options)
{
  callui(ui_open_tform, form, options);
}


/// Close tform (::ui_close_tform, only gui version).
/// \param form     pointer to the form to close
/// \param options  \ref FORM_CLOSE

inline void close_tform(TForm *form, int options)
{
  callui(ui_close_tform, form, options);
}


/// Activate tform (only gui version) (::ui_switchto_tform).
/// \param form        existing form to display
/// \param take_focus  give focus to given form

inline void switchto_tform(TForm *form, bool take_focus)
{
  callui(ui_switchto_tform, form, take_focus);
}


/// Find tform with the specified caption (only gui version) (::ui_find_tform).
/// NB: this callback works only with the tabbed forms!
/// \param caption  title of tab, or window title if tform is not tabbed
/// \return pointer to the TForm, NULL if none is found

inline TForm *find_tform(const char *caption)
{
  return (TForm *)callui(ui_find_tform, caption).vptr;
}


/// Get a pointer to the current tform (only gui version) (::ui_get_current_tform).
/// NB: this callback works only with the tabbed forms!

inline TForm *get_current_tform(void)
{
  return (TForm *)callui(ui_get_current_tform).vptr;
}


/// Get the type of the TForm * (::ui_get_tform_type).

inline tform_type_t get_tform_type(TForm *form)
{
  return tform_type_t(callui(ui_get_tform_type, form).i);
}


/// Get the TForm's title (::ui_get_tform_title).

inline bool get_tform_title(TForm *form, char *buf, size_t bufsize)
{
  return callui(ui_get_tform_title, form, buf, bufsize).cnd;
}


/// Create new ida viewer based on ::place_t (::ui_new_custom_viewer).
/// \param title     name of viewer
/// \param parent    form to hold viewer
/// \param minplace  first location of the viewer
/// \param maxplace  last location of the viewer
/// \param curplace  set current location
/// \param y         set current line # of cursor
/// \param ud        contents of viewer
/// \return pointer to resulting viewer

inline TCustomControl *create_custom_viewer(
        const char *title,
        TWinControl *parent,
        const place_t *minplace,
        const place_t *maxplace,
        const place_t *curplace,
        int y,
        void *ud)
{
  return (TCustomControl*)callui(ui_new_custom_viewer, title, parent,
                minplace, maxplace, curplace, y, ud).vptr;
}


/// Create a custom viewer with address support (::ui_new_ea_viewer).
/// For undocumented params see create_custom_viewer().
/// \param flags    reserved
/// \param fillloc  callback - used to fill current location data
/// \param jumploc  callback - called when the location changes

inline TCustomControl *create_ea_viewer(
        const char *title,
        TWinControl *parent,
        const place_t *minplace,
        const place_t *maxplace,
        const place_t *curplace,
        int y,
        void *ud,
        int flags=0,
        fillloc_t *fillloc=NULL,
        jumploc_t *jumploc=NULL)
{
  return (TCustomControl*)callui(ui_new_ea_viewer, title, parent,
                minplace, maxplace, curplace, y,
                ud, flags, fillloc, jumploc).vptr;
}


/// Push current location in the history and jump to the given location (::ui_ea_viewer_history_push_and_jump).
/// This will jump in the given ea viewer and also in other synchronized views.
/// \param v      ea viewer
/// \param ea     jump destination
/// \param x,y    coords on screen
/// \param lnnum  desired line number of given address

inline bool ea_viewer_history_push_and_jump(TCustomControl *v, ea_t ea, int x, int y, int lnnum)
{
  return callui(ui_ea_viewer_history_push_and_jump, v, ea, x, y, lnnum).cnd;
}


/// Get information about what's in the history (::ui_ea_viewer_history_info).
/// \param[out] nback  number of available back steps
/// \param[out] nfwd   number of available forward steps
/// \param v           ea viewer
/// \retval false  if the given ea viewer does not exist
/// \retval true   otherwise

inline bool get_ea_viewer_history_info(int *nback, int *nfwd, TCustomControl *v)
{
  return callui(ui_ea_viewer_history_info, nback, nfwd, v).cnd;
}


/// Refresh custom ida viewer (::ui_refresh_custom_viewer)

inline void refresh_custom_viewer(TCustomControl *custom_viewer)
{
  callui(ui_refresh_custom_viewer, custom_viewer);
}


/// Repaint the given widget immediately (::ui_repaint_qwidget)

inline void repaint_custom_viewer(TCustomControl *custom_viewer)
{
  callui(ui_repaint_qwidget, custom_viewer);
}


/// Destroy custom ida viewer

inline void destroy_custom_viewer(TCustomControl *custom_viewer)
{
  callui(ui_destroy_custom_viewer, custom_viewer);
}


/// Set cursor position in custom ida viewer.
/// \param custom_viewer view
/// \param place target position
/// \param x desired cursor position (column)
/// \param y desired cursor position (line)
/// \return success

inline bool jumpto(TCustomControl *custom_viewer, place_t *place, int x, int y)
{
  return callui(ui_jump_in_custom_viewer, custom_viewer, place, x, y).cnd;
}


/// Get current place in a custom viewer (::ui_get_curplace).
/// \param custom_viewer  view
/// \param mouse          mouse position (otherwise cursor position)
/// \param[out] x         x coordinate
/// \param[out] y         y coordinate

inline place_t *get_custom_viewer_place(
        TCustomControl *custom_viewer,
        bool mouse,
        int *x,
        int *y)
{
  return (place_t *)callui(ui_get_curplace, custom_viewer, mouse, x, y).vptr;
}


/// Are we running inside IDA Qt?

inline bool is_idaq()
{
  return callui(ui_clearbreak, KERNEL_VERSION_MAGIC1, KERNEL_VERSION_MAGIC2+2, NULL, NULL).cnd;
}


/// Insert a previously-registered action into the form's popup menu (::ui_attach_action_to_popup).
/// This function has two "modes": 'single-shot', and 'permanent'.
/// \param form          target form
/// \param popup_handle  target popup menu
///                        - if non-NULL, the action is added to this popup
///                          menu invocation (i.e., 'single-shot')
///                        - if NULL, the action is added to a list of actions
///                          that should always be present in context menus for this widget
///                          (i.e., 'permanent'.)
/// \param name          action name
/// \param popuppath     can be NULL
/// \param flags         a combination of SETMENU_ flags (see \ref SETMENU_)
/// \return success

inline bool attach_action_to_popup(
        TForm *form,
        TPopupMenu *popup_handle,
        const char *name,
        const char *popuppath = NULL,
        int flags = 0)
{
  return callui(ui_attach_action_to_popup, form, popup_handle, name, popuppath, flags).cnd;
}


/// Remove a previously-registered action, from the list of 'permanent'
/// context menu actions for this widget (::ui_detach_action_from_popup).
/// This only makes sense if the action has been added to 'form's list
/// of permanent popup actions by calling attach_action_to_popup
/// in 'permanent' mode.
/// \param form  target form
/// \param name  action name

inline bool detach_action_from_popup(TForm *form, const char *name)
{
  return callui(ui_detach_action_from_popup, form, name).cnd;
}


/// Create & insert an action into the form's popup menu (::ui_attach_dynamic_action_to_popup).
/// \note action_desc_t::handler for 'desc' must be instantiated using 'new', as it
/// will be 'delete'd when the action is unregistered.
/// \param form          target form
/// \param popup_handle  target popup
/// \param desc          created with #DYNACTION_DESC_LITERAL
/// \param popuppath     can be NULL
/// \param flags         a combination of SETMENU_ constants (see \ref SETMENU_)
/// \param buf           a buffer, to retrieve the generated action name - can be NULL
/// \param bufsize       the buffer size
/// \return success

inline bool attach_dynamic_action_to_popup(
        TForm *form,
        TPopupMenu *popup_handle,
        const action_desc_t &desc,
        const char *popuppath = NULL,
        int flags = 0,
        char *buf = NULL,
        size_t bufsize = size_t(-1))
{
  return callui(ui_attach_dynamic_action_to_popup, form,
                popup_handle, &desc, popuppath, flags,
                buf, bufsize).cnd;
}

/// \defgroup ui_uaa_funcs Functions: update actions
/// Convenience functions for ::ui_update_action_attr
//@{

/// Update an action's label (::ui_update_action_attr).
/// \param name   action name
/// \param label  new label
/// \return success

inline bool update_action_label(const char *name, const char *label)
{
  return callui(ui_update_action_attr, name, AA_LABEL, label).cnd;
}


/// Update an action's shortcut (::ui_update_action_attr).
/// \param name      action name
/// \param shortcut  new shortcut
/// \return success

inline bool update_action_shortcut(const char *name, const char *shortcut)
{
  return callui(ui_update_action_attr, name, AA_SHORTCUT, shortcut).cnd;
}


/// Update an action's tooltip (::ui_update_action_attr).
/// \param name     action name
/// \param tooltip  new tooltip
/// \return success

inline bool update_action_tooltip(const char *name, const char *tooltip)
{
  return callui(ui_update_action_attr, name, AA_TOOLTIP, tooltip).cnd;
}


/// Update an action's icon (::ui_update_action_attr).
/// \param name  action name
/// \param icon  new icon id
/// \return success

inline bool update_action_icon(const char *name, int icon)
{
  return callui(ui_update_action_attr, name, AA_ICON, &icon).cnd;
}


/// Update an action's state (::ui_update_action_attr).
/// \param name   action name
/// \param state  new state
/// \return success

inline bool update_action_state(const char *name, action_state_t state)
{
  return callui(ui_update_action_attr, name, AA_STATE, &state).cnd;
}


/// Update an action's checkability (::ui_update_action_attr).
/// \param name       action name
/// \param checkable  new checkability
/// \return success

inline bool update_action_checkable(const char *name, bool checkable)
{
  return callui(ui_update_action_attr, name, AA_CHECKABLE, &checkable).cnd;
}


/// Update an action's checked state (::ui_update_action_attr).
/// \param name     action name
/// \param checked  new checked state
/// \return success

inline bool update_action_checked(const char *name, bool checked)
{
  return callui(ui_update_action_attr, name, AA_CHECKED, &checked).cnd;
}


/// Update an action's visibility (::ui_update_action_attr).
/// \param name     action name
/// \param visible  new visibility
/// \return success

inline bool update_action_visibility(const char *name, bool visible)
{
  return callui(ui_update_action_attr, name, AA_VISIBILITY, &visible).cnd;
}

//@}

/// \defgroup ui_gaa_funcs Functions: get action attributes
/// Convenience functions for ::ui_get_action_attr
//{

/// Get an action's label (::ui_get_action_attr).
/// \param name        the action name
/// \param[out] label  the action label
/// \return success

inline bool get_action_label(const char *name, qstring *label)
{
  return callui(ui_get_action_attr, name, AA_LABEL, label).cnd;
}


/// Get an action's shortcut (::ui_get_action_attr).
/// \param name           the action name
/// \param[out] shortcut  the action shortcut
/// \return success

inline bool get_action_shortcut(const char *name, qstring *shortcut)
{
  return callui(ui_get_action_attr, name, AA_SHORTCUT, shortcut).cnd;
}


/// Get an action's tooltip (::ui_get_action_attr).
/// \param name          the action name
/// \param[out] tooltip  the action tooltip
/// \return success

inline bool get_action_tooltip(const char *name, qstring *tooltip)
{
  return callui(ui_get_action_attr, name, AA_TOOLTIP, tooltip).cnd;
}


/// Get an action's icon (::ui_get_action_attr).
/// \param name       the action name
/// \param[out] icon  the icon id
/// \return success

inline bool get_action_icon(const char *name, int *icon)
{
  return callui(ui_get_action_attr, name, AA_ICON, icon).cnd;
}


/// Get an action's state (::ui_get_action_attr).
/// \param name        the action name
/// \param[out] state  the action's state
/// \return success

inline bool get_action_state(const char *name, action_state_t *state)
{
  return callui(ui_get_action_attr, name, AA_STATE, state).cnd;
}


/// Get an action's checkability (::ui_get_action_attr).
/// \param name            the action name
/// \param[out] checkable  the action's checkability
/// \return success

inline bool get_action_checkable(const char *name, bool *checkable)
{
  return callui(ui_get_action_attr, name, AA_CHECKABLE, checkable).cnd;
}


/// Get an action's checked state (::ui_get_action_attr).
/// \param name          the action name
/// \param[out] checked  the action's checked state
/// \return success

inline bool get_action_checked(const char *name, bool *checked)
{
  return callui(ui_get_action_attr, name, AA_CHECKED, checked).cnd;
}


/// Get an action's visibility (::ui_get_action_attr).
/// \param name             the action name
/// \param[out] visibility  the action's visibility
/// \return success

inline bool get_action_visibility(const char *name, bool *visibility)
{
  return callui(ui_get_action_attr, name, AA_VISIBILITY, visibility).cnd;
}

//@}

/// \defgroup ui_scvh_funcs Functions: custom viewer handlers
/// Convenience functions for ::ui_set_custom_viewer_handler
//@{

/// Set handlers for custom viewer events
/// (::ui_set_custom_viewer_handlers - also see ::ui_set_custom_viewer_handler).
/// Any of these handlers may be NULL

inline void set_custom_viewer_handlers(
        TCustomControl *custom_viewer,
        custom_viewer_keydown_t *keyboard_handler,
        custom_viewer_popup_t *popup_handler,
        custom_viewer_mouse_moved_t *mouse_moved_handler,
        custom_viewer_dblclick_t *dblclick_handler,
        custom_viewer_curpos_t *curpos_handler,
        custom_viewer_close_t *close_handler,
        void *user_data)
{
  callui(ui_set_custom_viewer_handlers, custom_viewer, keyboard_handler,
         popup_handler, mouse_moved_handler, dblclick_handler, curpos_handler,
         close_handler, user_data);
}


/// Set a handler for a custom viewer event (::ui_set_custom_viewer_handler).
/// see also ::ui_set_custom_viewer_handlers
/// \param custom_viewer    the custom viewer
/// \param handler_id       one of CVH_ in ::custom_viewer_handler_id_t
/// \param handler_or_data  can be a handler or data. see examples in \ref ui_scvh_funcs
/// \return old value of the handler or data

inline void *set_custom_viewer_handler(
        TCustomControl *custom_viewer,
        custom_viewer_handler_id_t handler_id,
        void *handler_or_data)
{
  return callui(ui_set_custom_viewer_handler, custom_viewer, handler_id,
                handler_or_data).vptr;
}


/// Allow the given viewer to interpret Qt events (::ui_set_custom_viewer_handler)

inline bool set_custom_viewer_qt_aware(TCustomControl *custom_viewer)
{
  return callui(ui_set_custom_viewer_handler, custom_viewer, CVH_QT_AWARE).cnd;
}

//@}


/// Get current line of custom viewer (::ui_get_custom_viewer_curline).
/// The returned line contains color codes
/// \param custom_viewer  view
/// \param mouse          mouse position (otherwise cursor position)
/// \return pointer to contents of current line

inline const char *get_custom_viewer_curline(TCustomControl *custom_viewer, bool mouse)
{
  return callui(ui_get_custom_viewer_curline, custom_viewer, mouse).cptr;
}


/// Get current line of output window (::ui_get_output_curline).
/// \param buf      output buffer
/// \param bufsize  size of buffer
/// \param mouse    current for mouse pointer?
/// \return false if output contains no text

inline bool get_output_curline(char *buf, size_t bufsize, bool mouse)
{
  return callui(ui_get_output_curline, buf, bufsize, mouse).cnd;
}


/// Returns selected text from output window (::ui_get_output_selected_text).
/// \param buf      output buffer
/// \param bufsize  size of buffer
/// \return true if there is a selection

inline bool get_output_selected_text(char *buf, size_t bufsize)
{
  return callui(ui_get_output_selected_text, buf, bufsize).cnd;
}


/// Get the TCustomControl associated with the given TForm (::ui_get_tform_idaview).
/// \return pointer to the TCustomControl that is present in the TForm \n
///         NULL if the TForm doesn't host an IDA view.

inline TCustomControl *get_tform_idaview(TForm *parent)
{
  return (TCustomControl *)callui(ui_get_tform_idaview, parent).vptr;
}


/// Get current ida viewer (idaview or custom viewer) (::ui_get_current_viewer)

inline TCustomControl *get_current_viewer(void)
{
  return (TCustomControl *)callui(ui_get_current_viewer).vptr;
}


/// Get the type of renderer currently in use in the given view (::ui_get_renderer_type)

inline tcc_renderer_type_t get_view_renderer_type(TCustomControl *v)
{
  return tcc_renderer_type_t(callui(ui_get_renderer_type, v).i);
}


/// Set the type of renderer to use in a view (::ui_set_renderer_type)

inline void set_view_renderer_type(TCustomControl *v, tcc_renderer_type_t rt)
{
  callui(ui_set_renderer_type, v, rt);
}


/// Set position range for custom viewer (::ui_set_custom_viewer_range)

inline void set_custom_viewer_range(
        TCustomControl *custom_viewer,
        const place_t *minplace,
        const place_t *maxplace)
{
  callui(ui_set_custom_viewer_range, custom_viewer, minplace, maxplace);
}


/// Is the given custom view an idaview? (::ui_is_idaview)

inline bool is_idaview(TCustomControl *v)
{
  return callui(ui_is_idaview, v).cnd;
}


/// Get the name of the custom or ida viewer (::ui_get_viewer_name).
/// \param v        pointer to custom viewer
/// \param buf      output buffer
/// \param bufsize  size of buffer

inline void get_viewer_name(TCustomControl *v, char *buf, size_t bufsize)
{
  callui(ui_get_viewer_name, v, buf, bufsize);
}


/// Get the selected area boundaries (::ui_readsel2).
/// This is the more complex version of read_selection().
/// If you see only the addresses, use that instead.
/// \param v        view
/// \param[out] p1  start of selection
/// \param[out] p2  end of selection
/// \retval false   no area is selected
/// \retval true    ok, start and end are filled

inline bool readsel2(TCustomControl *v, twinpos_t *p1, twinpos_t *p2)
{
  return callui(ui_readsel2, v, p1, p2).cnd;
}


/// Create a code viewer (::ui_new_code_viewer).
/// \param parent    widget to contain the new code viewer
/// \param custview  the custom view to be added
/// \param flags     \ref CDVF_

inline TCustomControl *create_code_viewer(
        TForm *parent,
        TCustomControl *custview,
        int flags = 0)
{
  return (TCustomControl*)callui(ui_new_code_viewer, parent, custview, flags).vptr;
}


/// Set a handler for a code viewer event (::ui_set_custom_viewer_handler). \ingroup ui_scvh_funcs
/// \param code_viewer      the code viewer
/// \param handler_id       one of CDVH_ in ::custom_viewer_handler_id_t
/// \param handler_or_data  can be a handler or data. see examples in \ref ui_scvh_funcs
/// \return old value of the handler or data

inline void *set_code_viewer_handler(
        TCustomControl *code_viewer,
        custom_viewer_handler_id_t handler_id,
        void *handler_or_data)
{
  return callui(ui_set_custom_viewer_handler, code_viewer, handler_id,
                handler_or_data).vptr;
}


/// Set the user data on a code viewer (::ui_set_custom_viewer_handler). \ingroup ui_scvh_funcs

inline bool set_code_viewer_user_data(TCustomControl *code_viewer, void *ud)
{
  return callui(ui_set_custom_viewer_handler, code_viewer, CDVH_USERDATA, ud).cnd;
}


/// Get the user data from a custom viewer (::ui_get_viewer_user_data)

inline void *get_viewer_user_data(TCustomControl *viewer)
{
  return callui(ui_get_viewer_user_data, viewer).vptr;
}


/// Get the type of ::place_t instances a viewer uses & creates (::ui_get_viewer_place_type).

inline tcc_place_type_t get_viewer_place_type(TCustomControl *viewer)
{
  return tcc_place_type_t(callui(ui_get_viewer_place_type, viewer).i);
}


/// Set handlers for code viewer line events.
/// Any of these handlers may be NULL

inline void set_code_viewer_line_handlers(
        TCustomControl *code_viewer,
        code_viewer_lines_click_t *click_handler,
        code_viewer_lines_click_t *popup_handler,
        code_viewer_lines_click_t *dblclick_handler,
        code_viewer_lines_icon_t *drawicon_handler,
        code_viewer_lines_linenum_t *linenum_handler)
{
  callui(ui_set_code_viewer_line_handlers, code_viewer, click_handler,
         popup_handler, dblclick_handler, drawicon_handler, linenum_handler);
}


/// Set space allowed for icons in the margin of a code viewer (::ui_set_custom_viewer_handler). \ingroup ui_scvh_funcs

inline bool set_code_viewer_lines_icon_margin(TCustomControl *code_viewer, int margin)
{
  return callui(ui_set_custom_viewer_handler, code_viewer, CDVH_LINES_ICONMARGIN, margin).cnd;
}


/// Set alignment for lines in a code viewer (::ui_set_custom_viewer_handler). \ingroup ui_scvh_funcs

inline bool set_code_viewer_lines_alignment(TCustomControl *code_viewer, int align)
{
  return callui(ui_set_custom_viewer_handler, code_viewer, CDVH_LINES_ALIGNMENT, align).cnd;
}


/// Set radix for values displayed in a code viewer (::ui_set_custom_viewer_handler). \ingroup ui_scvh_funcs

inline bool set_code_viewer_lines_radix(TCustomControl *code_viewer, int radix)
{
  return callui(ui_set_custom_viewer_handler, code_viewer, CDVH_LINES_RADIX, radix).cnd;
}


/// Specify that the given code viewer is used to display source code (::ui_set_custom_viewer_handler). \ingroup ui_scvh_funcs

inline bool set_code_viewer_is_source(TCustomControl *code_viewer)
{
  return callui(ui_set_custom_viewer_handler, code_viewer, CDVH_SRCVIEW).cnd;
}


/// Get the size of a tab in spaces (::ui_get_tab_size).
/// \param path  the path of the source view for which the tab size is requested.
///                - if NULL, the default size is returned.

inline int get_tab_size(const char *path)
{
  return callui(ui_get_tab_size, path).i;
}


/// Clear Ctrl-Break flag (::ui_clearbreak)

THREAD_SAFE inline void clearBreak(void) { callui(ui_clearbreak); }


/// Set Ctrl-Break flag (::ui_setbreak)

THREAD_SAFE inline void setBreak(void) { callui(ui_setbreak); }


/// Test the ctrl-break flag (::ui_wasbreak).
/// \retval 1  Ctrl-Break is detected, a message is displayed
/// \retval 2  Ctrl-Break is detected again, a message is not displayed
/// \retval 0  Ctrl-Break is not detected

THREAD_SAFE inline bool wasBreak(void) { return callui(ui_wasbreak).cnd; }


/// Display a load file dialog and load file (::ui_load_file).
/// \param filename  the name of input file as is.
///                   (if the input file is from library, then this is the name in the library)
/// \param li        loader input source
/// \param neflags   combination of NEF_... bits (see \ref NEF_)
/// \retval true     file was successfully loaded
/// \retval false    otherwise

inline bool ui_load_new_file(const char *filename,
                             linput_t *li,
                             ushort neflags)
{
  return callui(ui_load_file, filename, li, neflags).cnd;
}


/// Load a debugger plugin and run the specified program (::ui_run_dbg).
/// \param dbgopts  value of the -r command line switch
/// \param exename  name of the file to run
/// \param argc     number of arguments for the executable
/// \param argv     argument vector
/// \return success

inline bool ui_run_debugger(const char *dbgopts,
                            const char *exename,
                            int argc,
                            const char *const *argv)
{
  return callui(ui_run_dbg, dbgopts, exename, argc, argv).cnd;
}


/// Load debugging information from a file.
/// \param path     path to file
/// \param li       loader input. if NULL, check DBG_NAME_KEY
/// \param base     loading address
/// \param verbose  dump status to message window

inline bool load_dbg_dbginfo(
        const char *path,
        linput_t *li=NULL,
        ea_t base=BADADDR,
        bool verbose=false)
{
  return callui(ui_load_dbg_dbginfo, path, li, base, verbose).cnd;
}


/// Add hotkey for IDC function (::ui_add_idckey).
/// \param hotkey   hotkey name
/// \param idcfunc  IDC function name
/// \return \ref IDCHK_

inline int add_idc_hotkey(const char *hotkey,const char *idcfunc)
{
  return callui(ui_add_idckey, hotkey, idcfunc).i;
}


/// Delete IDC function hotkey (::ui_del_idckey).
/// \param hotkey  hotkey name
/// \retval 1  ok
/// \retval 0  failed

inline bool del_idc_hotkey(const char *hotkey)
{
  return callui(ui_del_idckey, hotkey).cnd;
}


inline void get_user_strlist_options(strwinsetup_t *out)
{
  callui(ui_get_user_strlist_options, out);
}

#ifndef SWIG
/// Pointer to range marker function (for idaviews and hexviews)
/// This pointer is initialized by setup_range_marker()

extern void (idaapi*range_marker)(ea_t ea, asize_t size);


/// Initialize pointer to idaview marker

inline void setup_range_marker(void)
{
  void *ptr = callui(ui_get_range_marker).vptr;
  if ( ptr != NULL )
    range_marker = reinterpret_cast<void (idaapi*)(ea_t, asize_t)>(ptr);
}


/// Get the highlighted identifier in the current IDAView (::ui_get_highlighted_identifier).
/// \param buf      buffer to copy identifier to
/// \param bufsize  buffer size
/// \param flags    currently not used (pass 0)
/// \return false if no identifier is highlighted

inline bool get_highlighted_identifier(char *buf, size_t bufsize, int flags)
{
  return callui(ui_get_highlighted_identifier, buf, bufsize, flags).cnd;
}


/// Inform the UI about any modifications of [ea, ea+size)

inline void mark_range_for_refresh(ea_t ea, asize_t size)
{
  if ( range_marker != NULL )
    range_marker(ea, size);
}


/// Tell UI to refresh all idaviews and hexviews

inline void mark_all_eaviews_for_refresh(void)
{
  if ( range_marker != NULL )
    range_marker(0, BADADDR);
}
#endif // SWIG


/// \defgroup ui_open_builtin_funcs Functions: open built-in windows
/// Convenience functions for ::ui_open_builtin
//@{

/// Open the exports window (::ui_open_builtin).
/// \param ea  index of entry to select by default
/// \return pointer to resulting window

inline TForm *open_exports_window(ea_t ea)
{
  return (TForm *)callui(ui_open_builtin, BWN_EXPORTS, ea).vptr;
}


/// Open the exports window (::ui_open_builtin).
/// \param ea  index of entry to select by default
/// \return pointer to resulting window

inline TForm *open_imports_window(ea_t ea)
{
  return (TForm *)callui(ui_open_builtin, BWN_IMPORTS, ea).vptr;
}


/// Open the names window (::ui_open_builtin).
/// \param ea  index of entry to select by default
/// \return pointer to resulting window

inline TForm *open_names_window(ea_t ea)
{
  return (TForm *)callui(ui_open_builtin, BWN_NAMES, ea).vptr;
}


/// Open the functions window (::ui_open_builtin).
/// \param ea  index of entry to select by default
/// \return pointer to resulting window

inline TForm *open_funcs_window(ea_t ea)
{
  return (TForm *)callui(ui_open_builtin, BWN_FUNCS, ea).vptr;
}


/// Open the strings window (::ui_open_builtin).
/// \param ea                index of entry to select by default
/// \param selstart,selend   only display strings that occur within this range
/// \return pointer to resulting window

inline TForm *open_strings_window(ea_t ea, ea_t selstart=BADADDR, ea_t selend=BADADDR)
{
  return (TForm *)callui(ui_open_builtin, BWN_STRINGS, ea, selstart, selend).vptr;
}


/// Open the segments window (::ui_open_builtin).
/// \param ea  index of entry to select by default
/// \return pointer to resulting window

inline TForm *open_segments_window(ea_t ea)
{
  return (TForm *)callui(ui_open_builtin, BWN_SEGS, ea).vptr;
}


/// Open the segment registers window (::ui_open_builtin).
/// \param ea  index of entry to select by default
/// \return pointer to resulting window

inline TForm *open_segregs_window(ea_t ea)
{
  return (TForm *)callui(ui_open_builtin, BWN_SEGREGS, ea).vptr;
}


/// Open the selectors window (::ui_open_builtin).
/// \return pointer to resulting window

inline TForm *open_selectors_window(void)
{
  return (TForm *)callui(ui_open_builtin, BWN_SELS, 0).vptr;
}


/// Open the signatures window (::ui_open_builtin).
/// \return pointer to resulting window

inline TForm *open_signatures_window(void)
{
  return (TForm *)callui(ui_open_builtin, BWN_SIGNS, 0).vptr;
}


/// Open the type libraries window (::ui_open_builtin).
/// \return pointer to resulting window

inline TForm *open_tils_window(void)
{
  return (TForm *)callui(ui_open_builtin, BWN_TILS, 0).vptr;
}


/// Open the local types window (::ui_open_builtin).
/// \param ordinal  ordinal of type to select by default
/// \return pointer to resulting window

inline TForm *open_loctypes_window(int ordinal)
{
  return (TForm *)callui(ui_open_builtin, BWN_LOCTYPS, ordinal).vptr;
}


/// Open the function calls window (::ui_open_builtin).
/// \return pointer to resulting window

inline TForm *open_calls_window(ea_t ea)
{
  return (TForm *)callui(ui_open_builtin, BWN_CALLS, ea).vptr;
}

/// Open the problems window (::ui_open_builtin).
/// \param ea  index of entry to select by default
/// \return pointer to resulting window

inline TForm *open_problems_window(ea_t ea)
{
  return (TForm *)callui(ui_open_builtin, BWN_PROBS, ea).vptr;
}


/// Open the breakpoints window (::ui_open_builtin).
/// \param ea  index of entry to select by default
/// \return pointer to resulting window

inline TForm *open_bpts_window(ea_t ea)
{
  return (TForm *)callui(ui_open_builtin, BWN_BPTS, ea).vptr;
}


/// Open the threads window (::ui_open_builtin).
/// \return pointer to resulting window

inline TForm *open_threads_window(void)
{
  return (TForm *)callui(ui_open_builtin, BWN_THREADS, 0).vptr;
}


/// Open the modules window (::ui_open_builtin).
/// \return pointer to resulting window

inline TForm *open_modules_window(void)
{
  return (TForm *)callui(ui_open_builtin, BWN_MODULES, 0).vptr;
}


/// Open the trace window (::ui_open_builtin).
/// \return pointer to resulting window

inline TForm *open_trace_window(void)
{
  return (TForm *)callui(ui_open_builtin, BWN_TRACE, 0).vptr;
}


/// Open the call stack window (::ui_open_builtin).
/// \return pointer to resulting window

inline TForm *open_stack_window(void)
{
  return (TForm *)callui(ui_open_builtin, BWN_STACK, 0).vptr;
}


/// Open the cross references window (::ui_open_builtin).
/// \param ea  index of entry to select by default
/// \return pointer to resulting window

inline TForm *open_xrefs_window(ea_t ea)
{
  return (TForm *)callui(ui_open_builtin, BWN_XREFS, ea).vptr;
}


/// Open the frame window for the given function (::ui_open_builtin).
/// \param pfn     function to analyze
/// \param offset  offset where the cursor is placed
/// \return pointer to resulting window if 'pfn' is a valid function and the window was displayed,  \n
///                 NULL otherwise

inline TForm *open_frame_window(func_t *pfn, uval_t offset)
{
  return (TForm *)callui(ui_open_builtin, BWN_FRAME, pfn, offset).vptr;
}


/// Open the navigation band window (::ui_open_builtin).
/// \param ea    sets the address of the navband arrow
/// \param zoom  sets the navband zoom level
/// \return pointer to resulting window

inline TForm *open_navband_window(ea_t ea, int zoom)
{
  return (TForm *)callui(ui_open_builtin, BWN_NAVBAND, ea, zoom).vptr;
}


/// Open the enums window (::ui_open_builtin).
/// \param const_id  index of entry to select by default
/// \return pointer to resulting window

inline TForm *open_enums_window(tid_t const_id=BADADDR)
{
  return (TForm *)callui(ui_open_builtin, BWN_ENUMS, const_id).vptr;
}


/// Open the structs window (::ui_open_builtin).
/// \param id      index of entry to select by default
/// \param offset  offset where the cursor is placed
/// \return pointer to resulting window

inline TForm *open_structs_window(tid_t id=BADADDR, uval_t offset=0)
{
  return (TForm *)callui(ui_open_builtin, BWN_STRUCTS, id, offset).vptr;
}


/// Open a disassembly view (::ui_open_builtin).
/// \param window_title  title of view to open
/// \param ranges        if != NULL, then display a flow chart with the specified ranges
/// \return pointer to resulting window

inline TForm *open_disasm_window(const char *window_title, const areavec_t *ranges=NULL)
{
  return (TForm *)callui(ui_open_builtin, BWN_DISASMS, window_title, BADADDR, ranges, 0).vptr;
}


/// Open a hexdump view (::ui_open_builtin).
/// \param window_title  title of view to open
/// \return pointer to resulting window

inline TForm *open_hexdump_window(const char *window_title)
{
  return (TForm *)callui(ui_open_builtin, BWN_DUMPS, window_title, BADADDR, 0).vptr;
}


/// Open the notepad window (::ui_open_builtin).
/// \return pointer to resulting window

inline TForm *open_notepad_window(void)
{
  return (TForm *)callui(ui_open_builtin, BWN_NOTEPAD, 0).vptr;
}

//@}


/// \defgroup ui_choose_funcs Functions: built-in choosers
/// Convenience functions for ::ui_choose and ::choose_type_t
//@{


/// Choose a signature (::ui_choose, ::chtype_idasgn).
/// \return name of selected signature, NULL if none selected

inline char *choose_idasgn(void)
{
  return callui(ui_choose, chtype_idasgn).cptr;
}


/// Choose a type library (::ui_choose, ::chtype_idatil).
/// \param buf      output buffer to store the library name
/// \param bufsize  size of output buffer
/// \retval true   'buf' was filled with the name of the selected til
/// \retval false  otherwise

inline bool choose_til(char *buf, size_t bufsize)
{
  return callui(ui_choose, chtype_idatil, buf, bufsize).cnd;
}


/// Choose an entry point (::ui_choose, ::chtype_entry).
/// \param title  chooser title
/// \return ea of selected entry point, #BADADDR if none selected

inline ea_t choose_entry(const char *title)
{
  ea_t ea;
  callui(ui_choose, chtype_entry, &ea, title);
  return ea;
}


/// Choose a name (::ui_choose, ::chtype_name).
/// \param title  chooser title
/// \return ea of selected name, #BADADDR if none selected

inline ea_t choose_name(const char *title)
{
  ea_t ea;
  callui(ui_choose, chtype_name, &ea, title);
  return ea;
}


/// Choose an xref to a stack variable (::ui_choose, ::chtype_name).
/// \param pfn   function
/// \param mptr  variable
/// \return ea of the selected xref, BADADDR if none selected

inline ea_t choose_stkvar_xref(func_t *pfn, member_t *mptr)
{
  ea_t ea;
  callui(ui_choose, chtype_stkvar_xref, &ea, pfn, mptr);
  return ea;
}


/// Choose an xref to an address (::ui_choose, ::chtype_xref).
/// \param to  referenced address
/// \return ea of selected xref, BADADDR if none selected

inline ea_t choose_xref(ea_t to)
{
  ea_t ea;
  callui(ui_choose, chtype_xref, &ea, to);
  return ea;
}


/// Choose an enum (::ui_choose, ::chtype_enum).
/// \param title       chooser title
/// \param default_id  id of enum to select by default
/// \return enum id of selected enum, #BADNODE if none selected

inline enum_t choose_enum(const char *title, enum_t default_id)
{
  enum_t enum_id = default_id;
  callui(ui_choose, chtype_enum, &enum_id, title);
  return enum_id;
}


/// Choose an enum, restricted by value & size (::ui_choose, ::chtype_enum_by_value_and_size).
/// If the given value cannot be found initially, this function will
/// ask if the user would like to import a standard enum.
/// \param title        chooser title
/// \param default_id   id of enum to select by default
/// \param value        value to search for
/// \param nbytes       size of value
/// \param[out] serial  serial number of imported enum member, if one was found
/// \return enum id of selected (or imported) enum, #BADNODE if none was found

inline enum_t choose_enum_by_value(
        const char *title,
        enum_t default_id,
        uval_t value,
        int nbytes,
        uchar *serial)
{
  enum_t enum_id = default_id;
  callui(ui_choose, chtype_enum_by_value_and_size, &enum_id, title, value, nbytes, serial);
  return enum_id;
}


/// Choose a function (::ui_choose, ::chtype_func).
/// \param title       chooser title
/// \param default_ea  ea of function to select by default
/// \return pointer to function that was selected, NULL if none selected

inline func_t *choose_func(const char *title, ea_t default_ea)
{
  return callui(ui_choose, chtype_func, title, default_ea).fptr;
}


/// Choose a segment (::ui_choose, ::chtype_segm).
/// \param title       chooser title
/// \param default_ea  ea of segment to select by default
/// \return pointer to segment that was selected, NULL if none selected

inline segment_t *choose_segm(const char *title, ea_t default_ea)
{
  return callui(ui_choose, chtype_segm, title, default_ea).segptr;
}


/// Choose a structure (::ui_choose, ::chtype_segm).
/// \param title  chooser title;
/// \return pointer to structure that was selected, NULL if none selected

inline struc_t *choose_struc(const char *title)
{
  return callui(ui_choose, chtype_struc, title).strptr;
}


/// Choose a segment register change point (::ui_choose, ::chtype_srcp).
/// \param title  chooser title
/// \return pointer to segment register area of selected change point, NULL if none selected

inline segreg_area_t *choose_srcp(const char *title)
{
  return callui(ui_choose, chtype_srcp, title).sraptr;
}

//@}

#ifndef SWIG

/// Get path to a structure offset (for nested structures/enums) (::ui_choose, ::chtype_strpath).

inline int choose_struc_path(const char *title, tid_t strid,
                      uval_t offset, adiff_t delta, bool appzero, tid_t *path)
{
  return callui(ui_choose, chtype_strpath, title, strid,
                                            offset, delta, appzero, path).i;
}

// Generic list chooser (1-column).
// See the description of this function above in this file
// (to find it, search for "list choosers")
inline uint32 choose(
        int flags,
        int x0,int y0,
        int x1,int y1,
        void *obj,
        int width,
        uint32 (idaapi*sizer)(void *obj),
        char *(idaapi*getl)(void *obj,uint32 n,char *buf),
        const char *title,
        int icon,
        uint32 deflt,
        chooser_cb_t *del,
        void (idaapi*ins)(void *obj),
        chooser_cb_t *update,
        void (idaapi*edit)(void *obj,uint32 n),
        void (idaapi*enter)(void * obj,uint32 n),
        void (idaapi*destroy)(void *obj),
        const char * const *popup_names,
        int (idaapi*get_icon)(void *obj,uint32 n))
{
  return callui(ui_choose, chtype_generic, flags, x0, y0, x1, y1, obj, width,
                sizer, getl, title, icon, deflt, del, ins, update,
                edit, enter, destroy, popup_names, get_icon).i32;
}


// Generic list chooser (n-column).
// See the description of this function above in this file
// (to find it, search for "list choosers")
inline uint32 choose2(
        int flags,
        int x0,int y0,
        int x1,int y1,
        void *obj,
        int ncol,
        const int *widths,
        uint32 (idaapi*sizer)(void *obj),
        void (idaapi*getl)(void *obj,uint32 n,char * const *arrptr),
        const char *title,
        int icon,
        uint32 deflt,
        chooser_cb_t *del,
        void (idaapi*ins)(void *obj),
        chooser_cb_t *update,
        void (idaapi*edit)(void *obj,uint32 n),
        void (idaapi*enter)(void * obj,uint32 n),
        void (idaapi*destroy)(void *obj),
        const char * const *popup_names,
        int (idaapi*get_icon)(void *obj,uint32 n))
{
  return callui(ui_choose, chtype_generic2, flags, x0, y0, x1, y1, obj, ncol,
                widths, sizer, getl, title, icon, deflt, del, ins,
                update, edit, enter, destroy, popup_names, get_icon).i32;
}

/// Invoke the chooser with a chooser_info_t structure (::ui_choose_info).
/// \retval 0 no chosen item (may be ESC or header line)
/// \retval >0 number of chosen item
/// \retval -1 chooser already exists (non-modal windows)

inline uint32 choose3(const chooser_info_t *chi)
{
  return callui(ui_choose_info, chi).i32;
}

#endif // SWIG


/// Get the underlying object of the specified chooser (::ui_get_chooser_obj).
/// NB: for single-column choosers, a proxy object will be returned.
/// \return the object that was used to create the chooser

inline void *get_chooser_obj(const char *chooser_caption)
{
  return callui(ui_get_chooser_obj, chooser_caption).vptr;
}


/// Enable item-specific attributes for chooser items (::ui_enable_chooser_item_attrs).
/// For example: color list items differently depending on a criterium.             \n
/// If enabled, the chooser will generate ui_get_chooser_item_attrs                 \n
/// events that can be intercepted by a plugin to modify the item attributes.       \n
/// This event is generated only in the GUI version of IDA.                         \n
/// Specifying #CH_ATTRS bit at the chooser creation time has the same effect.
/// \return success

inline bool idaapi enable_chooser_item_attrs(const char *chooser_caption, bool enable)
{
  return callui(ui_enable_chooser_item_attrs, chooser_caption, enable).cnd;
}


/// See show_wait_box()

THREAD_SAFE AS_PRINTF(1, 0) inline void show_wait_box_v(const char *format, va_list va)
{
  callui(ui_mbox, mbox_wait, format, va);
}


/// Display a dialog box with "Please wait...".
/// If the text message starts with "HIDECANCEL\n", the cancel button       \n
/// won't be displayed in the dialog box and you don't need to check        \n
/// for cancellations with wasBreak(). Plugins must call hide_wait_box()    \n
/// to close the dialog box, otherwise the user interface will be disabled.
///
/// Note that, if the wait dialog is already visible, show_wait_box() will  \n
///   1) push the currently-displayed text on a stack                       \n
///   2) display the new text                                               \n
/// Then, when hide_wait_box() is called, if that stack isn't empty its top \n
/// label will be popped and restored in the wait dialog.                   \n
/// This implies that a plugin should call hide_wait_box() exactly as many  \n
/// times as it called show_wait_box(), or the wait dialog might remain     \n
/// visible and block the UI.                                               \n
/// Also, in case the plugin knows the wait dialog is currently displayed,  \n
/// alternatively it can call replace_wait_box(), to replace the text of the\n
/// dialog without pushing the currently-displayed text on the stack.
THREAD_SAFE AS_PRINTF(1, 2) inline void show_wait_box(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  show_wait_box_v(format, va);
  va_end(va);
}


/// Hide the "Please wait dialog box"

THREAD_SAFE inline void hide_wait_box(void)
{
  // stupid watcom requires va_list should not be NULL
  callui(ui_mbox, mbox_hide, NULL, &callui);
}


/// Replace the label of "Please wait dialog box"

THREAD_SAFE AS_PRINTF(1, 2) inline void replace_wait_box(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  callui(ui_mbox, mbox_replace, format, va);
  va_end(va);
}


/// See AskUsingForm_c()

inline int AskUsingForm_cv(const char *format, va_list va)
{
  return callui(ui_form, format, va).i;
}


/// Create and/or activate dockable modeless form (::ui_show_form).
/// \param format  string
/// \param flags   \ref FORM_OPEN
/// \param va      args
/// \return pointer to resulting TForm

inline TForm *OpenForm_cv(const char *format, int flags, va_list va)
{
  return (TForm *)callui(ui_show_form, format, flags, va).vptr;
}


/// Close the form.
/// A helper function for the complex dialog box (form) callbacks.
/// This function may be called from pushbutton callbacks in ::ui_form.
/// \param fields           array of form fields
/// \param close_normally   control how the form is closed
///                           - 1: form is closed normally as if the user pressed Enter
///                           - 0: form is closed abnormally as if the user pressed Esc

inline void close_form(const TView *const *fields, int close_normally)
{
  callui(ui_close_form, fields, close_normally);
}


/// Issue a beeping sound (::ui_beep).
/// \param beep_type  ::beep_t

inline void beep(beep_t beep_type=beep_default)
{
  callui(ui_beep, beep_type);
}


/// See askbuttons_c()

THREAD_SAFE AS_PRINTF(5, 0) inline int askbuttons_cv(
        const char *Yes,
        const char *No,
        const char *Cancel,
        int deflt,
        const char *format,
        va_list va)
{
  return callui(ui_askyn, Yes, No, Cancel, deflt, format, va).i;
}


/// See askfile_c()

AS_PRINTF(3, 0) inline char *askfile_cv(
        int savefile,
        const char *defval,
        const char *format,
        va_list va)
{
  return callui(ui_askfile, savefile, defval, format, va).cptr;
}


/// See askfile2_c()

AS_PRINTF(4, 0) inline char *askfile2_cv(
        bool forsave,
        const char *defdir,
        const char *filters,
        const char *format,
        va_list va)
{
  return callui(ui_askfile2, forsave, defdir, filters, format, va).cptr;
}


/// Show a message box asking to send the input file to support@hex-rays.com.
/// \param format  the reason why the input file is bad

THREAD_SAFE AS_PRINTF(1, 2) inline void ask_for_feedback(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  callui(ui_mbox, mbox_feedback, format, va);
  va_end(va);
}


/// Display a dialog box and wait for the user to input an identifier (::ui_askident).
/// If the user enters a non-valid identifier, this function displays a warning
/// and allows to correct it.
/// \param defval   default value. will be displayed initially in the input line.
///                 may be NULL.
/// \param format   number of message from IDA.HLP to display as question
///                 in printf() style format
/// \return NULL-if the user pressed Esc.
///         otherwise returns pointer to identifier.

AS_PRINTF(2, 3) inline char *askident(const char *defval, const char *format, ...)
{
  va_list va;
  va_start(va, format);
  char *result = callui(ui_askident, defval, format, va).cptr;
  va_end(va);
  return result;
}


/// Display a dialog box and wait for the user to input an address (::ui_askaddr).
/// \param addr     in/out parameter. contains pointer to the address.
/// \param format   printf() style format string with the question
/// \retval 0  the user pressed Esc.
/// \retval 1  ok, the user entered an address

AS_PRINTF(2, 3) inline bool askaddr(ea_t *addr, const char *format, ...)
{
  va_list va;
  va_start(va, format);
  bool ok = callui(ui_askaddr, addr, format, va).cnd;
  va_end(va);
  return ok;
}


/// Display a dialog box and wait for the user to input an segment name (::ui_askseg).
/// This function allows to enter segment register names, segment base
/// paragraphs, segment names to denote a segment.
/// \param sel      in/out parameter. contains selector of the segment
/// \param format   printf() style format string with the question
/// \retval  0  if the user pressed Esc.  \n
/// \retval  1  ok, the user entered an segment name

AS_PRINTF(2, 3) inline bool askseg(sel_t *sel, const char *format, ...)
{
  va_list va;
  va_start(va, format);
  bool ok = callui(ui_askseg, sel, format, va).cnd;
  va_end(va);
  return ok;
}


/// Display a dialog box and wait for the user to input an number (::ui_asklong).
/// The number is represented in C-style.
/// This function allows to enter any IDC expression and
/// properly calculates it.
/// \param value    in/out parameter. contains pointer to the number
/// \param format   printf() style format string with the question
/// \retval 0  if the user pressed Esc.  \n
/// \retval 1  -ok, the user entered a valid number.

AS_PRINTF(2, 3) inline bool asklong(sval_t *value, const char *format, ...)
{
  va_list va;
  va_start(va, format);
  bool ok = callui(ui_asklong, value, format, va).cnd;
  va_end(va);
  return ok;
}


/// See askstr()

AS_PRINTF(3, 0) inline char *vaskstr(
        int hist,
        const char *defval,
        const char *format,
        va_list va)
{
  return callui(ui_askstr, hist, defval, format, va).cptr;
}

/// See askqstr()

AS_PRINTF(2, 0) inline bool vaskqstr(
  qstring *str,
  const char *format,
  va_list va)
{
  return callui(ui_askqstr, str, format, va).cnd;
}

/// See asktext()

AS_PRINTF(4, 0) inline char *vasktext(
        size_t size,
        char *answer,
        const char *defval,
        const char *format,
        va_list va)
{
  return callui(ui_asktext, size, answer, defval, format, va).cptr;
}


/// See warning()

THREAD_SAFE AS_PRINTF(1, 0) inline void vwarning(const char *format, va_list va)
{
  callui(ui_mbox, mbox_warning, format, va);
}


/// See info()

THREAD_SAFE AS_PRINTF(1, 0) inline void vinfo(const char *format, va_list va)
{
  callui(ui_mbox, mbox_info, format, va);
}


/// See nomem()

THREAD_SAFE AS_PRINTF(1, 0) NORETURN inline void vnomem(const char *format, va_list va)
{
  callui(ui_mbox, mbox_nomem, format, va);
  // NOTREACHED
#ifndef UNDER_CE
  abort(); // to suppress compiler warning or error
#endif
}


/// See msg()

THREAD_SAFE AS_PRINTF(1, 0) inline int vmsg(const char *format, va_list va)
{
  return callui(ui_msg, format, va).i;
}


/// See umsg()

THREAD_SAFE AS_PRINTF(1, 0) inline int vumsg(const char *format, va_list va)
{
  return callui(ui_umsg, format, va).i;
}


/// Display copyright warning (::ui_copywarn).
/// \return yes/no

inline bool display_copyright_warning(void)
{
  return callui(ui_copywarn).cnd;
}

#ifndef NO_OBSOLETE_FUNCS
inline DEPRECATED void add_custom_viewer_popup_item(
        TCustomControl *v,
        const char *t,
        const char *h,
        menu_item_callback_t *c,
        void *u)
{
  callui(ui_obsolete_add_custom_viewer_popup, v, t, h, c, u);
}

inline DEPRECATED bool add_menu_item(
        const char *m,
        const char *n,
        const char *h,
        int f,
        menu_item_callback_t *c,
        void *u)
{
  return callui(ui_obsolete_add_menu_item, m, n, h, f, c, u).cnd;
}

inline DEPRECATED void add_output_popup(char *n, menu_item_callback_t *c, void *u)
{
  callui(ui_obsolete_add_output_popup, n, c, u);
}

inline DEPRECATED bool del_menu_item(const char *m)
{
  return callui(ui_obsolete_del_menu_item, m).cnd;
}

inline DEPRECATED void set_custom_viewer_popup_menu(
        TCustomControl *custom_viewer,
        TPopupMenu *menu)
{
  callui(ui_obsolete_set_custom_viewer_popup, custom_viewer, menu);
}

inline DEPRECATED bool add_chooser_command(
        const char *chooser_caption,
        const char *cmd_caption,
        chooser_cb_t *chooser_cb,
        int menu_index=-1,
        int icon=-1,
        int flags=0)
{
  return callui(ui_obsolete_add_chooser_cmd, chooser_caption, cmd_caption, chooser_cb,
                menu_index, icon, flags).cnd;
}

inline DEPRECATED bool add_chooser_command(
        const char *chooser_caption,
        const char *cmd_caption,
        chooser_cb_t *chooser_cb,
        const char *hotkey,
        int menu_index=-1,
        int icon=-1,
        int flags=0)
{
  return callui(ui_obsolete_add_chooser_cmd, chooser_caption, cmd_caption, chooser_cb,
                menu_index, icon, flags|CHOOSER_HOTKEY, hotkey).cnd;
}

class segreg_t;
inline DEPRECATED segreg_t *choose_segreg(const char *title)
{
  return (segreg_t *)callui(ui_choose, chtype_segreg, title).sregptr;
}
#endif // NO_OBSOLETE_FUNCS

#endif  // __UI__ END OF UI SERVICE FUNCTIONS

//---------------------------------------------------------------------------
//      E R R O R / W A R N I N G / I N F O   D I A L O G   B O X E S
//---------------------------------------------------------------------------

/// If this variable is set, then dialog boxes will not appear on the screen.
/// Warning/info messages are shown in the messages window.           \n
/// The default value of user input dialogs will be returned to the
/// caller immediately.                                               \n
/// This variable is used to enable unattended work of ida.
idaman int ida_export_data batch;


idaman int ida_export_data errorexit;   ///< useful for atexit() functions,
                                        ///< is 1 if we are exiting with from
                                        ///< error() function.


/// Display error dialog box and exit.
/// Err() function does the same but the format string is taken from IDA.HLP.
/// If you just want to display an error message and let IDA continue,
/// do NOT use this function! Use warning() instead.
/// \param format  printf() style message string.
///                It may have some prefixes, see 'Format of dialog box' for details.

THREAD_SAFE AS_PRINTF(1, 2) NORETURN inline void error(const char *format,...)
{
  va_list va;
  va_start(va, format);
  verror(format, va);
  // NOTREACHED
}


/// Display warning dialog box and wait for the user to press Enter or Esc.
/// This messagebox will by default contain a "Don't display this message again"  \n
/// checkbox if the message is repetitively displayed. If checked, the message    \n
/// won't be displayed anymore during the current IDA session.                    \n
/// Warn() function does the same but the format string is taken from IDA.HLP
/// \param format  printf() style format string.
///                It may have some prefixes, see 'Format of dialog box' for details.

THREAD_SAFE AS_PRINTF(1, 2) inline void warning(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  vwarning(format, va);
  va_end(va);
}


/// Display info dialog box and wait for the user to press Enter or Esc.
/// This messagebox will by default contain a "Don't display this message again"    \n
/// checkbox. If checked, the message will never be displayed anymore (state saved  \n
/// in the Windows registry or the idareg.cfg file for a non-Windows version).      \n
/// Info() function does the same but the format string is taken from IDA.HLP
/// \param format  printf() style format string.
///                It may have some prefixes, see 'Format of dialog box' for details.

THREAD_SAFE AS_PRINTF(1, 2) inline void info(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  vinfo(format, va);
  va_end(va);
}


/// Display "no memory for module ..." dialog box and exit.
/// \param format   printf() style message string.

THREAD_SAFE AS_PRINTF(1, 2) NORETURN inline void nomem(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  vnomem(format, va);
  // NOTREACHED
}


/// Output a formatted string to messages window [analog of printf()].
/// Message() function does the same but the format string is taken from IDA.HLP
/// Everything appearing on the messages window may be written
/// to a text file. For this the user should define the following environment
/// variable:                       \n
///         set IDALOG=idalog.txt
///
/// \param format  printf() style message string.
/// \return number of bytes output

THREAD_SAFE AS_PRINTF(1, 2) inline int msg(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  int nbytes = vmsg(format, va);
  va_end(va);
  return nbytes;
}


/// Show a message in the message window (::ui_umsg, also see ::ui_msg).
/// All strings ('format', and those that will be used for
/// '%s' format specifiers), MUST be utf-8-encoded strings.
/// \return number of bytes output

THREAD_SAFE AS_PRINTF(1, 2) inline int umsg(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  int nbytes = vumsg(format, va);
  va_end(va);
  return nbytes;
}

#ifndef SWIG

/*! \defgroup FORM_C AskUsingForm_c()/OpenForm_c()

  \brief This module describes how to generate a custom form.

  <pre>

  The following keywords might appear at the beginning of the 'form' argument
  (case insensitive):

  STARTITEM number

    where number is a number of input field the cursor will stand on.
    By default the cursor is in the first field of the dialog box.
    The input fields are numbered from 0 (the first field is field 0).

  BUTTON name caption

    Alternative caption for a button. It may contain the character
    to highlight in this form:  ~Y~es
    Valid button names are: YES, NO, CANCEL
    For example:
        BUTTON YES Please do
        BUTTON NO Nope
        BUTTON CANCEL NONE

    By default the NO button is not displayed. If it is displayed, then
    the return value of the function will be different!
    (see the function description)

    Empty text means that there won't be any corresponding button.
    (you may also use NONE as the caption to hide it)

    A * after the button name means that this button will be the default:

      BUTTON CANCEL* Cancel

  Next, if the dialog box is kept in IDA.HLP, the following may appear:
  (this defines help context for the whole dialog box)

  @hlpMessageName[]

  If the form is not in IDA.HLP file, then it can have a built-in
  help message. In this case the help screen should be enclosed in the
  following keywords:

  HELP
  ....
  ....
  ....
  ENDHELP

  Each keyword should be on a separate line.

  Next there must be the title line and two empty lines.
  Most of the text in the dialog box text string is copied to the dialog
  without modification. There are three special cases:

        - dynamic labels (format parameters)
        - callback arguments
        - input fields

  For example, this dialog box:

  ------ format:
        Sample dialog box


        This is sample dialog box for %A
        using address %$

        <~E~nter value:N:32:16::>

  ------

  Contains two dynamic labels (text %A and address %$) and one input field
  (numerical input box with the label "Enter value").

  Parameters for the dynamic labels and input fields are taken from the
  function's input arguments (va_list). The corresponding argument should
  contain a pointer (sic, pointer) to the value to be displayed.

  The dialog box above should be called as

                \code
                char *string = "something";
                ea_t addr = someaddr;
                uval_t answer = 0;
                int ok = AskUsingForm_c(format, string, &addr, &answer);
                \endcode


  Dynamic labels are used to specify variant parts of the dialog box text.
  They use the following syntax:

        %nT

  where
        n  - optional decimal field ID, which may be used in the
             ::form_actions_t calls to get/set label value at runtime
        T  - a character specifying type of input field. All input field
             types (except B and K) are valid format specifiers. See below
             for the list.


  There are two special specifiers for callbacks:

  The combination '%/' corresponds to a callback function that will be
  called when any of the fields is modified. The callback type is ::formchgcb_t.
  There can be only one such callback.

  The combination '%*' is used to store user data (void *) in the form.
  This data can be later retrieved from the ::formchgcb_t callback via the
  form action method get_ud().

  Input fields use the following syntax:

  <label:type:width:swidth:@hlp[]>

  where
        label - any text string serving as label for the input field
                the label may contain hotkey definition like this: "~O~pen"
                (O keystroke is hotkey here)
        type  - a character specifying type of input field.
                The form() function will perform initial validation of
                value specified by the user and convert it appropriately.
                See table of input field types below. The type can be followed
                by a decimal number, an input field ID.
        width - for A, I, T, X: decimal number specifying size of the buffer
                  passed for text input fields (including terminating 0).
                  if omitted or <0, assumed to be at least MAXSTR

                for B, k: the code generated when the user presses the button (passed to the button callback)
                for f (path to file) this attribute specifies the dialog type:
                  0-'open file' dialog box
                  1-'save file' dialog box
                for F (folder) it is ignored (buffer is assumed to be at least QMAXPATH long)
                for b (dropdown list) this attribute specifies the readonly attribute:
                  0   - read-only dropdown list
                  > 0 - editable dropdown list
                for n, N, Y, O, H, M, D, L, l, S, $, q: decimal number specifying maximum
                  possible number of characters that can be entered into the input field
                for the rest of controls: this field is ignored
        swidth -decimal number specifying width of visible part of input field.
                this number may be omitted.
        @hlp[]- help context for the input field. you may replace the
                help context with '::' (two colons) if you don't want to
                specify help context. The help context is a number of help
                screen from IDA.HLP file.


  Input field types                               va_list parameter
  -----------------                               -----------------

  A - ascii string                                char* at least MAXSTR size
  q - ascii string                                ::qstring*
  S - segment                                     ::sel_t*
  N - hex number, C notation                      ::uval_t*
  n - signed hex number, C notation               ::sval_t*
  L - C notation number                           ::uint64*
      (prefix 0x - hex, 0 - octal, otherwise decimal)
  l - same as L but with optional sign            ::int64*
  M - hex number, no "0x" prefix                  ::uval_t*
  D - decimal number                              ::sval_t*
  O - octal number, C notation                    ::sval_t*
  Y - binary number, "0b" prefix                  ::sval_t*
  H - char value, C notation                      ::sval_t*
  $ - address                                     ::ea_t*
  I - ident                                       char* at least #MAXNAMELEN size
  B - button                                      ::formcb_t
  k - txt: button (same as B)/gui: hyperlink      ::formcb_t
  K - color button                                ::bgcolor_t*
  F - path to folder                              char* at least #QMAXPATH size
  f - path to file                                char* at least #QMAXPATH size
  T - type declaration                            char* at least #MAXSTR size
  X - command                                     char* at least #MAXSTR size
  E - chooser                                     ::chooser_info_t * - Embedded chooser
                                                  ::intvec_t * - in/out: selected lines
                                                    (NB: this field takes two args)
  t - multi line text control                     ::textctrl_info_t *
  b - dropdown list                               ::qstrvec_t * - the list of items
                                                  int* or ::qstring* - the preselected item
                                                    (::qstring* when the combo is editable, i.e. width field is >0)

  The M, n, N, D, O, Y, H, $ fields try to parse the input as an IDC expression
  and convert the result into the required value type

  If the buffer for 'F' field contains filemasks and descriptions like this:
    *.exe|Executable files,*.dll|Dll files
  they will be used in the dialog box filter.

  The hint message can be specified before the label enclosed in '#':

  <#hint message#label:...>

  Radiobuttons and checkboxes are represented by:

  <label:type>
  <label:type>>         - end of block

  where valid types are C and R
  (you may use lowercase 'c' and 'r' if you need to create two radiobutton
  or checkbox groups on the same lines). The field ID of the whole group
  can be specified between the brackets: <label:type>ID>

  field types           va_list parameter
  -----------           -----------------

  C - checkbox          ushort*                 bit mask of checkboxes
  R - radiobutton       ushort*                 number of radiobutton

  The box title and hint messages can be specified like this:

  <#item hint#title#box hint#label:type>

  The title and the box hint can be specified only in the first item of the box.
  If the hint doesn't exist, it should be specified as an empty hint (##title##)
  The subsequent items can have an item hint only:

  <#item hint#label:type>

  Initial values of input fields are specified in the corresponding
  input/output parameters (taken from va_list array).

  OK, Cancel and (possibly) Help buttons are displayed at the bottom of
  the dialog box automatically. Their captions can be changed by the BUTTON
  keywords described at the beginning of this page.

  Input field definition examples:

   <Kernel analyzer options ~1~:B:0:::>
   <~A~nalysis enabled:C>
   <~I~ndicator enabled:C>>
   <Names pre~f~ix  :A:15:15::>
   <~O~utput file:f:1:64::>
   <~O~utput directory:F::64::>

  Resizable fields can be separated by splitter (GUI  only) represented by <|>
  Splitter usage example:
   <~Chooser~:E1:0:40:::><|><~E~ditor:t2:0:40:::>

  </pre>
*/
//@{
//----------------------------------------------------------------------
//      F O R M S  -  C O M P L E X   D I A L O G   B O X E S
//----------------------------------------------------------------------

/// See AskUsingForm_c()

int AskUsingForm_cv(const char *form, va_list va);


/// Display a dialog box and wait for the user.
/// If the form contains the "BUTTON NO <title>" keyword, then the return values
/// are the same as in the askyn() function (\ref ASKBTN_)
/// \param form  dialog box as a string. see \ref FORM_C
/// \retval 0    the user pressed Esc, no memory to display or form syntax error
///                a dialog box (a warning is displayed in this case).
///                all variables retain their original values.
/// \retval 1    ok, all input fields are filled and validated.

inline int AskUsingForm_c(const char *form, ...)
{
  va_list va;
  va_start(va, form);
  int code = AskUsingForm_cv(form, va);
  va_end(va);
  return code;
}

#ifndef __UI__

/// See OpenForm_c()

TForm *OpenForm_cv(const char *form, int flags, va_list va);


//----------------------------------------------------------------------
/// Display a dockable modeless dialog box and return a handle to it.
/// \param form      dialog box as a string. see \ref FORM_C
/// \param flags     \ref FORM_OPEN
/// \return handle to the form or NULL.
///         the handle can be used with TForm functions: close_tform()/switchto_tform()/etc

inline TForm *OpenForm_c(const char *form, int flags, ...)
{
  va_list va;
  va_start(va, flags);
  TForm *tform = OpenForm_cv(form, flags, va);
  va_end(va);
  return tform;
}
#endif

//@} FORM_C


/// Callback: called when the user presses a form-specific button (defined with a field type B).
/// Note: from this callback you may call close_form() function.  \n
/// In this case the form will be closed as soon as you return from the callback.
/// \param fields  array of input fields
/// \param code    code of the pressed button

typedef void (idaapi *formcb_t)(TView *fields[], int code); // callback for buttons

/// Functions available from ::formchgcb_t.
/// For getters/setters for specific field values, see #DEF_SET_METHOD.
struct form_actions_t
{
  /// Get value of an input field.
  /// \return false if no such field id or invalid field type (B)
  virtual bool idaapi _get_field_value(int field_id, void *buf) = 0;

  /// Set value of an input field.
  /// \return false if no such field id or invalid field type (B)
  virtual bool idaapi _set_field_value(int field_id, const void *buf) = 0;

  /// Enable or disable an input field.
  /// \return false if no such field id
  virtual bool idaapi enable_field(int field_id, bool enable) = 0;

  /// Show or hide an input field.
  /// \return false if no such field id
  virtual bool idaapi show_field(int field_id, bool display) = 0;

  /// Move/Resize an input field.
  /// Parameters specified as -1 are not modified.
  /// \return false no such field id
  virtual bool idaapi move_field(int field_id, int x, int y, int w, int h) = 0;

  /// Get currently focused input field.
  /// \return -1 if no such field
  virtual int idaapi get_focused_field(void) = 0;

  /// Set currently focused input field.
  /// \return false if no such field id
  virtual bool idaapi set_focused_field(int field_id) = 0;

  /// Refresh a field
  virtual void idaapi refresh_field(int field_id) = 0;

  /// Close the form (see close_form())
  virtual void idaapi close(int close_normally) = 0;

  /// Retrieve the user data specified through %*
  virtual void *idaapi get_ud() = 0;

  /// Get value of an ascii string input field.
  /// \return false if no such field id or invalid field type (B)
  virtual bool idaapi _get_str_field_value (int field_id, char *buf, const size_t bufsize) = 0;

/// Helper to define functions in ::form_actions_t that get/set field values of different types.
/// Please see this file's source code for specific uses.
#define DEF_SET_METHOD(NAME, TYPE)                                          \
  inline bool idaapi set_ ## NAME ## _value(int field_id, const TYPE *val)  \
  {                                                                         \
    return _set_field_value(field_id, val);                                 \
  }
/// \copydoc DEF_SET_METHOD
#define DEF_FIELD_METHOD(NAME, TYPE)                                        \
  inline bool idaapi get_ ## NAME ## _value(int field_id, TYPE *val)        \
  {                                                                         \
    return _get_field_value(field_id, val);                                 \
  }                                                                         \
  DEF_SET_METHOD(NAME, TYPE)
/// \copydoc DEF_SET_METHOD
#define DEF_STR_FIELD_METHOD(NAME            )                              \
  inline bool idaapi get_ ## NAME ## _value(int field_id, char *buf, const size_t bufsize) \
  {                                                                         \
    return _get_str_field_value(field_id, buf, bufsize);                    \
  }                                                                         \
  DEF_SET_METHOD(NAME, char)

  // get/set value of radio button (r, R)
  DEF_FIELD_METHOD(radiobutton, ushort)
  // get/set value of radio button group
  DEF_FIELD_METHOD(rbgroup, ushort)
  // get/set value of check box (c, C)
  DEF_FIELD_METHOD(checkbox, ushort)
  // get/set value of check box group
  DEF_FIELD_METHOD(cbgroup, ushort)
  // get/set value of color control (K)
  DEF_FIELD_METHOD(color, bgcolor_t)
  // get/set embedded chooser selected items (E)
  DEF_FIELD_METHOD(chooser, intvec_t)
  // get/set value of editable combo box (b when field 'width' >0)
  DEF_FIELD_METHOD(combobox, qstring)
  // get/set selected item of read-only combo box (b when field 'width' ==0)
  DEF_FIELD_METHOD(combobox, int)
  // get/set value of multiline text input control (t)
  DEF_FIELD_METHOD(text, textctrl_info_t)
  // get/set value of dynamic label (%)
  DEF_STR_FIELD_METHOD(label)
  // get/set value of ascii string (A, I, T)
  DEF_STR_FIELD_METHOD(ascii)
  // get/set string value(q)
  DEF_FIELD_METHOD(string, qstring)
  // get/set value of segment (S)
  DEF_FIELD_METHOD(segment, sel_t)
  // get/set signed value (n,D,O,Y,H)
  DEF_FIELD_METHOD(signed, sval_t)
  // get/set unsigned value (N, M)
  DEF_FIELD_METHOD(unsigned, uval_t)
  // get/set value of default base (usually hex) number (l)
  DEF_FIELD_METHOD(int64, int64)
  // get/set value of default base (usually hex) number (L)
  DEF_FIELD_METHOD(uint64, uint64)
  // get/set address value ($)
  DEF_FIELD_METHOD(ea, ea_t)
  // get/set path value (F,f)
  DEF_STR_FIELD_METHOD(path)
  // get/set identifier value (I)
  DEF_STR_FIELD_METHOD(ident)

#undef DEF_FIELD_METHOD
#undef DEF_SET_METHOD
#undef DEF_STR_FIELD_METHOD
};


/// Callback. Called when an input field is modified.
/// The callback will be also called before displaying the form and as soon
/// as the user presses OK.
/// \param field_id  id of the modified field
/// \retval -1       form is going to be displayed
/// \retval -2       form is going to be closed with OK.
/// \retval >0       form will be closed

typedef int (idaapi *formchgcb_t)(int field_id, form_actions_t &fa);


#endif // SWIG

//---------------------------------------------------------------------------
//      Y E S / N O   D I A L O G  B O X
//---------------------------------------------------------------------------

/// \defgroup ASKBTN_ Button IDs
/// used by askyn_c() and askbuttons_c()
//@{
#define ASKBTN_YES     1  ///< Yes button
#define ASKBTN_NO      0  ///< No button
#define ASKBTN_CANCEL -1  ///< Cancel button
#define ASKBTN_BTN1    1  ///< First (Yes) button
#define ASKBTN_BTN2    0  ///< Second (No) button
#define ASKBTN_BTN3   -1  ///< Third (Cancel) button
//@}


/// See askyn_c()

AS_PRINTF(2, 0) inline int askyn_cv(int deflt, const char *format, va_list va)
{
  return askbuttons_cv(NULL, NULL, NULL, deflt, format, va);
}


/// Display a dialog box and get choice from "Yes", "No", "Cancel".
/// \param deflt    default choice: one of \ref ASKBTN_
/// \param format   The question in printf() style format
/// \return the selected button (one of \ref ASKBTN_). Esc key returns #ASKBTN_CANCEL.

AS_PRINTF(2, 3) inline int askyn_c(int deflt, const char *format, ...)
{
  va_list va;
  va_start(va, format);
  int code = askyn_cv(deflt, format, va);
  va_end(va);
  return code;
}


/// Display a dialog box and get choice from maximum three possibilities (::ui_askyn).
/// \note for all buttons:
///   - use "" or NULL to take the default name for the button.
///   - use 'format' to hide the cancel button
/// \param Yes     text for the first button
/// \param No      text for the second button
/// \param Cancel  text for the third button
/// \param deflt   default choice: one of \ref ASKBTN_
/// \param format  printf-style format string for question. It may have some prefixes, see below.
/// \param va      parameters for the format string
/// \return one of \ref ASKBTN_ specifying the selected button (Esc key returns Cancel/3rd button value)

AS_PRINTF(5, 6) inline int askbuttons_c(
        const char *Yes,
        const char *No,
        const char *Cancel,
        int deflt,
        const char *format,
        ...)
{
  va_list va;
  va_start(va, format);
  int code = askbuttons_cv(Yes, No, Cancel, deflt, format, va);
  va_end(va);
  return code;
}

//------------------------------------------------------------------------
/* Format of dialog box (actually they are mutliline strings
                         delimited by newline characters)

  The very first line of dialog box can specify a dialog box
  title if the following line appears:

  TITLE title string


  Then, the next line may contain an icon to display
  in the GUI version (ignored by the text version):

  ICON NONE          (no icon)
       INFO          (information icon)
       QUESTION      (question icon)
       WARNING       (warning icon)
       ERROR         (error icon)


  Then, the next line may contain a 'Don't display this message again'
  checkbox. If this checkbox is selected and the user didn't select cancel,
  the button he selected is saved and automatically returned.

  AUTOHIDE NONE      (no checkbox)
           DATABASE  (return value is saved to database)
           REGISTRY  (return value is saved to Windows registry or idareg.cfg
                      if non-Windows version)
           SESSION   (return value is saved for the current IDA session)
  It is possible to append "*" to the AUTOHIDE keywords to have this checkbox
  initially checked. For example: "AUTOHIDE REGISTRY*"

  To hide the cancel button the following keyword can be used:

  HIDECANCEL

  Please note that the user still can cancel the dialog box by pressing Esc
  or clicking on the 'close window' button.

  Finally, if the dialog box is kept in IDA.HLP, the following may appear
  to add a Help button (this defines help context for the whole dialog box):

  @hlpMessageName[]


  Each keyword should be alone on a line.

  Next, a format string must be specified.
  To center message lines in the text version, start them with '\3' character
  (currently ignored in the GUI version).
*/

//---------------------------------------------------------------------------
//      A S K   S T R I N G   O F   T E X T
//---------------------------------------------------------------------------


/// Display a dialog box and wait for the user to input a text string (::ui_askstr).
/// Use this function to ask one-line text. For multiline input, use
/// asktext() and asksmalltext() functions.
/// This function will trim the trailing spaces.
/// \param hist     category of history lines. an arbitrary number.           \n
///                   this number determines lines accessible in the history  \n
///                   of the user input (when he presses down arrow)          \n
///                   One of \ref HIST_ should be used here
/// \param defval   default value. will be displayed initially in the input line.
///                   may be NULL.
/// \param format   printf() style format string with the question
/// \return NULL-if the user pressed Esc.
///         otherwise returns the entered value in a static storage.

AS_PRINTF(3, 4) inline char *askstr(int hist, const char *defval, const char *format, ...)
{
  va_list va;
  va_start(va, format);
  char *result = vaskstr(hist, defval, format, va);
  va_end(va);
  return result;
}

/// \defgroup HIST_ Input line history constants
/// passed as 'hist' parameter to askstr()
//@{
#define HIST_SEG    1           ///< segment names
#define HIST_CMT    2           ///< comments
#define HIST_SRCH   3           ///< search substrings
#define HIST_ADDR   4           ///< addresses
#define HIST_IDENT  5           ///< names
#define HIST_NUM    6           ///< numbers
#define HIST_FILE   7           ///< file names
#define HIST_TYPE   8           ///< type declarations
#define HIST_CMD    9           ///< commands
#define HIST_DIR   10           ///< directory names
//@}

/// Display a dialog box and wait for the user to input a text string (::ui_askstr).
/// Use this function to ask one-line text. For multiline input, use
/// asktext() and asksmalltext() functions.
/// This function will trim the trailing spaces.
/// \param str      qstring to fill. Can contain default value. Cannot be NULL.
/// \param format   printf() style format string with the question
/// \return false-if the user pressed Esc, otherwise returns  true.

AS_PRINTF(2, 3) inline bool askqstr(qstring *str, const char *format, ...)
{
  va_list va;
  va_start(va, format);
  bool result = vaskqstr(str, format, va);
  va_end(va);
  return result;
}

#ifndef SWIG


/// Display a dialog box and wait for the user to input multiline text (::ui_asktext).
/// \param size     maximum size of text in bytes
/// \param answer   output buffer. if you specify NULL then the answer
///                   will be returned in a buffer allocated by
///                   'new char' operator.
/// \param defval   default value. will be displayed initially in the input line.
///                   may be NULL.
/// \param format   printf() style format string with the question
/// \return NULL-if the user pressed Esc.
///         otherwise returns pointer to text.

AS_PRINTF(4, 5) inline char *asktext(
        size_t size,
        char *answer,
        const char *defval,
        const char *format,
        ...)
{
  va_list va;
  va_start(va, format);
  char *result = vasktext(size, answer, defval, format, va);
  va_end(va);
  return result;
}
#endif // SWIG


//---------------------------------------------------------------------------
//      A S K   A D D R E S S E S ,   N A M E S ,   N U M B E R S ,   E T C .
//---------------------------------------------------------------------------

/// Display a dialog box and wait for the user to input a file name (::ui_askfile).
/// This function displays a window with file names present in the directory
/// pointed to by 'defval'.
/// \param savefile  nonzero means filename will be used to save a file
/// \param defval    default value. will be displayed initially in the input line.
///                    may be NULL may be or a wildcard file name.
/// \param format    printf-style format string with the question
/// \return NULL-if the user pressed Esc.
///         Otherwise the user entered a valid file name.

AS_PRINTF(3, 4) inline char *askfile_c(
        int savefile,
        const char *defval,
        const char *format,
        ...)
{
  va_list va;
  va_start(va, format);
  char *answer = askfile_cv(savefile, defval, format, va);
  va_end(va);
  return answer;
}


/// Display a dialog box and wait for the user to input a file name (::ui_askfile2).
/// This function displays a window with file names present in the directory
/// pointed by 'defval'.
/// \param forsave  the entered file name will be used to save file
/// \param defdir   default location
/// \param filters  filters in the format 'description1|wildcard11;wildcards12;...|description2|wildcards21;wildcards22...|...',
///                   or the name of a file
/// \param format   printf() style format string with the question
/// \retval pointer to the resulting filename
/// \retval NULL the user pressed Esc.

AS_PRINTF(4, 5) inline char *askfile2_c(
        bool forsave,
        const char *defdir,
        const char *filters,
        const char *format,
        ...)
{
  va_list va;
  va_start(va, format);
  char *answer = askfile2_cv(forsave, defdir, filters, format, va);
  va_end(va);
  return answer;
}


//---------------------------------------------------------------------------
//      A D D - O N S
//---------------------------------------------------------------------------

/// Information about an installed add-on (e.g. a plugin)
struct addon_info_t
{
  size_t cb;                //< size of this structure
  const char *id;           //< product code, e.g. "com.hexrays.hexx86w". Must be unique
  const char *name;         //< descriptive name, e.g. "Hex-Rays x86 Decompiler (Windows)"
  const char *producer;     //< e.g. "Hex-Rays SA"
  const char *version;      //< version string, e.g. 1.5.110408
  const char *url;          //< URL of the product http://www.hex-rays.com/decompiler.shtml
  const char *freeform;     //< any string, e.g. "Copyright (c) 2007-2011 Hex-Rays"
  const void *custom_data;  //< custom data (license ID etc). Can be NULL. Not displayed in UI.
  size_t custom_size;

  /// Constructor
  addon_info_t() { memset(this, 0, sizeof(addon_info_t)); cb = sizeof(addon_info_t); };
};

#ifndef __UI__

/// \defgroup ui_addons_funcs Functions: add-ons
/// Convenience functions for ::ui_addons
//@{

/// Register an add-on. Show its info in the About box.
/// For plugins, should be called from init() function
/// (repeated calls with the same product code overwrite previous entries)
/// returns: index of the add-on in the list, or -1 on error

inline int register_addon(const addon_info_t *info)
{
  return callui(ui_addons, 0, info).i;
}


/// Get number of installed addons

inline int addon_count()
{
  return callui(ui_addons, 1).i;
}


/// Get info about a registered addon with a given product code.
/// info->cb must be valid!
/// NB: all pointers are invalidated by next call to register_addon or get_addon_info
/// \return false if not found

inline bool get_addon_info(const char *id, addon_info_t *info)
{
  return callui(ui_addons, 2, id, info).cnd;
}


/// Get info about a registered addon with specific index.
/// info->cb must be valid!
/// NB: all pointers are invalidated by next call to register_addon or get_addon_info
/// \return false if index is out of range

inline bool get_addon_info_idx(int index, addon_info_t *info)
{
  return callui(ui_addons, 3, index, info).cnd;
}

//@} ui_addons_funcs

#endif

//---------------------------------------------------------------------------
//      S T R I N G   F U N C T I O N S
//---------------------------------------------------------------------------
/// \defgroup str_funcs Functions: strings
/// functions that manipulate strings
//@{

/// Add space characters to the colored string so that its length will be at least
/// 'len' characters. Don't trim the string if it is longer than 'len'.
/// \param str      pointer to colored string to modify (may not be NULL)
/// \param bufsize  size of the buffer with the string
/// \param len      the desired length of the string
/// \return pointer to the end of input string

idaman THREAD_SAFE char *ida_export add_spaces(char *str, size_t bufsize, ssize_t len);


/// Remove trailing space characters from a string.
/// \param str  pointer to string to modify (may be NULL)
/// \return pointer to input string

idaman THREAD_SAFE char *ida_export trim(char *str);


/// Skip whitespaces in the string.
/// \return pointer to first non-whitespace char in given string

idaman THREAD_SAFE const char *ida_export skipSpaces(const char *ptr);
inline char *skipSpaces(char *ptr) ///< \copydoc skipSpaces()
  { return CONST_CAST(char*)(skipSpaces((const char *)ptr)); }

/// Map strings to integer values - see strarray()
struct strarray_t
{
  int code;
  const char *text;
};


/// Find a line with the specified code in the ::strarray_t array.
/// If the last element of the array has code==0 then it is considered as the default entry.  \n
/// If no default entry exists and the code is not found, strarray() returns "".

idaman THREAD_SAFE const char *ida_export strarray(const strarray_t *array, size_t array_size, int code);


#ifndef SWIG

/// Convert whitespace to tabulations.
/// This function will stop the conversion as soon as a string or character constant
/// is encountered.

idaman THREAD_SAFE void ida_export entab(char *string);


//---------------------------------------------------------------------------
//      C O N V E R S I O N S
//---------------------------------------------------------------------------
/// \defgroup conv Functions: string conversion
/// functions that convert between string encodings
//@{

/// Convert linear address to ascii string

idaman size_t ida_export ea2str(ea_t ea, char *buf, size_t bufsize);


inline bool ea2str(qstring *out, ea_t ea)
{
  char tmp[MAXSTR];
  if ( ea2str(ea, tmp, sizeof(tmp)) <= 0 )
    return false;
  *out = tmp;
  return true;
}


/// Convert string to linear address.
/// Tries to interpret the string as:                                                                       \n
/// 1) "current IP" keyword if supported by assembler (e.g. "$" in x86)                                     \n
/// 2) segment:offset expression, where "segment" may be a name or a fixed segment register (e.g. cs, ds)   \n
/// 3) just segment name/register (translated to segment's start address)                                   \n
/// 4) a name in the database (or debug name during debugging)                                              \n
/// 5) +delta or -delta, where numerical 'delta' is added to or subtracted from 'screenEA'                  \n
/// 6) if all else fails, try to evaluate 'str' as an IDC expression

idaman bool ida_export str2ea(const char *str, ea_t *ea_ptr, ea_t screenEA);


/// Same as str2ea() but possibly with some steps skipped.
/// \param flags  \ref S2EAOPT_

idaman bool ida_export str2ea_ex(const char *str, ea_t *ea_ptr, ea_t screenEA, int flags);

/// \defgroup S2EAOPT_ String to address conversion flags
/// passed as 'flags' parameter to str2ea_ex()
//@{
#define S2EAOPT_NOCALC 0x00000001 ///< don't try to interpret string as IDC (or current extlang) expression
//@}


/// Convert a number in C notation to an address.
/// decimal: 1234         \n
/// octal: 0123           \n
/// hexadecimal: 0xabcd   \n
/// binary: 0b00101010

idaman bool ida_export atoea(const char *str, ea_t *pea);


/// Convert segment selector to ascii string

idaman size_t ida_export stoa(ea_t from, sel_t seg, char *buf, size_t bufsize);


/// Convert ascii string to segment selector.
/// \retval 0 - fail
/// \retval 1 - ok (hex)
/// \retval 2 - ok (segment name or reg)

idaman int ida_export atos(const char *str, sel_t *seg);


#define MAX_NUMBUF (128+8) ///< 16-byte value in binary base (0b00101010...)


/// Get the number of ascii characters required to represent
/// a number with the specified number of bytes and radix.
/// \param radix   if 0, use default radix, usually 16
/// \param nbytes  if 0, use default number of bytes, usually 4 or 8 depending on __EA64__

idaman size_t ida_export b2a_width(int nbytes, int radix);


/// Convert number to ascii string (includes leading zeroes).
/// \param x        value to convert
/// \param buf      output buffer
/// \param bufsize  size of output buffer
/// \param nbytes   1, 2, 3, or 4
/// \param radix    2, 8, 10, or 16
/// \return size of resulting string

idaman size_t ida_export b2a32(uint32 x, char *buf, size_t bufsize, int nbytes, int radix);


/// Same as b2a32(), but can handle 'nbytes' = 8

idaman size_t ida_export b2a64(uint64 x, char *buf, size_t bufsize, int nbytes, int radix);


/// Get max number of ascii characters required to represent
/// a given type of value, with a given size (without leading zeroes).
/// \param nbytes  size of number
/// \param flag    should be one of FF_ for #MS_0TYPE
/// \param n       if 1, shr 'flag' by 4

idaman size_t ida_export btoa_width(int nbytes, flags_t flag, int n);


/// Same as b2a32(), but will generate a string without any leading zeroes.
/// Can be used to output some numbers in the instructions.

idaman size_t ida_export btoa32(char *buf, size_t bufsize, uint32 x, int radix=0);


/// 64-bit equivalent of btoa32()

idaman size_t ida_export btoa64(char *buf, size_t bufsize, uint64 x, int radix=0);


/// 128-bit equivalent of btoa32()

idaman size_t ida_export btoa128(char *buf, size_t bufsize, uint128 x, int radix=0);

#ifdef __EA64__
#define b2a b2a64
#define btoa btoa64
#define atob atob64
#else
#define b2a b2a32    ///< shortcut for number->string conversion, see b2a32()
#define btoa btoa32  ///< shortcut for number->string conversion, see btoa32()
#define atob atob32  ///< shortcut for string->number conversion, see atob32()
#endif


/// Convert instruction operand immediate number to ascii.
/// This is the main function to output numbers in the instruction operands.         \n
/// It prints the number with or without the leading zeroes depending on the flags.  \n
/// This function is called from OutValue(). please use OutValue() if you can.

idaman size_t ida_export numop2str(char *buf,
                                   size_t bufsize,
                                   ea_t ea,
                                   int n,
                                   uint64 x,
                                   int nbytes,
                                   int radix=0);


/// Convert ascii to a number using the current assembler formats.
/// e.g. for ibmpc, '12o' is octal, '12h' is hex, etc.
/// \return success

idaman bool ida_export atob32(const char *str, uint32 *x);


/// 64-bit equivalent of atob32()

idaman bool ida_export atob64(const char *str, uint64 *x); // returns 1-ok


/// Auxiliary function.
/// Print displacement to a name (+disp or -disp) in the natural radix
/// \param buf   pointer to the output buffer
/// \param end   pointer to the end of the output buffer
/// \param disp  displacement to output. 0 leads to no modifications
/// \return length of resulting string

idaman size_t ida_export print_disp(char *buf, char *end, adiff_t disp);


/// \defgroup c_str_conv Functions: c strings
/// String C-style conversions (convert \\n to a newline and vice versa)
//@{
idaman THREAD_SAFE char *ida_export str2user(char *dst, const char *src, size_t dstsize); ///< Make a user representation
idaman THREAD_SAFE char *ida_export user2str(char *dst, const char *src, size_t dstsize); ///< Make an internal representation
idaman THREAD_SAFE char ida_export back_char(const char *&p);                             ///< Translate char after '\\'
idaman THREAD_SAFE void ida_export qstr2user(qstring *dst, const qstring &src);           ///< see str2user()
idaman THREAD_SAFE void ida_export user2qstr(qstring *dst, const qstring &src);           ///< see user2str()
//@}

/// Convert RADIX50 -> ASCII.
/// \param r  pointer to radix50 string
/// \param p  pointer to ascii string
/// \param k  number of elements in the input string                      \n
///           (element of radix50 string is a word)                       \n
///           (element of ascii   string is a character)
/// \return   number of elements left unprocessed in the input string,    \n
///           because the input string contains unconvertible elements.   \n
///           0-ok, all elements are converted

idaman THREAD_SAFE int ida_export r50_to_asc(const ushort *r, char *p, int k);


/// Convert ASCII -> RADIX50 (see r50_to_asc())

int THREAD_SAFE asc_to_r50(const char *p, ushort *r, int k);


/// Maintain a collection of characters that are allowed in ASCII strings.
/// In order to find the end of a string IDA looks for a character
/// which doesn't belong to this array.

idaman char ida_export_data AsciiStringChars[256+1];

//@} Conversion functions
//@} String functions

/// \defgroup pack Pack/Unpack
/// Functions for packing and unpacking values
//{

/// Pack a byte into a character string.
/// This function encodes numbers using an encoding similar to UTF.
/// The smaller the number, the better the packing.
/// \param ptr  pointer to output buffer
/// \param end  pointer to end of output buffer
/// \param x    value to pack
/// \return pointer to end of resulting string

THREAD_SAFE inline uchar *idaapi pack_db(uchar *ptr, uchar *end, uchar x)
{
  if ( ptr < end )
    *ptr++ = x;
  return ptr;
}


/// Unpack a byte from a character string, pack_db()

THREAD_SAFE inline uchar idaapi unpack_db(const uchar **pptr, const uchar *end)
{
  const uchar *ptr = *pptr;
  uchar x = 0;
  if ( ptr < end )
    x = *ptr++;
  *pptr = ptr;
  return x;
}

idaman THREAD_SAFE uchar *ida_export pack_dw(uchar *ptr, uchar *end, uint16 x); ///< pack a word, see pack_db()
idaman THREAD_SAFE uchar *ida_export pack_dd(uchar *ptr, uchar *end, uint32 x); ///< pack a double word, see pack_db()
idaman THREAD_SAFE uchar *ida_export pack_dq(uchar *ptr, uchar *end, uint64 x); ///< pack a quadword, see pack_db()
idaman THREAD_SAFE ushort ida_export unpack_dw(const uchar **pptr, const uchar *end); ///< unpack a word, see unpack_db()
idaman THREAD_SAFE uint32 ida_export unpack_dd(const uchar **pptr, const uchar *end); ///< unpack a double word, see unpack_db()
idaman THREAD_SAFE uint64 ida_export unpack_dq(const uchar **pptr, const uchar *end); ///< unpack a quadword, see unpack_db()

/// Pack an ea value into a character string, see pack_dd()/pack_dq()

THREAD_SAFE inline uchar *pack_ea(uchar *ptr, uchar *end, ea_t ea)
{
#ifdef __EA64__
  return pack_dq(ptr, end, ea);
#else
  return pack_dd(ptr, end, ea);
#endif
}

/// Unpack an ea value, see unpack_dd()/unpack_dq()

THREAD_SAFE inline ea_t unpack_ea(const uchar **ptr, const uchar *end)
{
#ifdef __EA64__
  return unpack_dq(ptr, end);
#else
  return unpack_dd(ptr, end);
#endif
}


/// Pack a string.
/// \param ptr  pointer to output buffer
/// \param end  pointer to end of output buffer
/// \param x    string to pack. If NULL, empty string is packed
/// \param len  number of chars to pack. If 0, the length of given string is used
/// \return pointer to end of packed string

idaman THREAD_SAFE uchar *ida_export pack_ds(uchar *ptr, uchar *end, const char *x, size_t len=0);


/// Unpack a string.
/// \param pptr        pointer to packed string
/// \param end         pointer to end of packed string
/// \param empty_null  if true, then return NULL for empty strings.   \n
///                    otherwise return an empty string (not NULL).
/// \return pointer to unpacked string.                               \n
///         this string will be allocated in dynamic memory.          \n
///         the caller should use qfree() to deallocate it.

idaman THREAD_SAFE char  *ida_export unpack_ds(const uchar **pptr, const uchar *end, bool empty_null);


/// Unpack an object of a known size.
/// \param pptr      pointer to packed object
/// \param end       pointer to end of packed object
/// \param destbuf   output buffer
/// \param destsize  size of output buffer
/// \return pointer to the destination buffer.
///         if any error, returns NULL.

THREAD_SAFE inline void *idaapi unpack_obj(const uchar **pptr, const uchar *end, void *destbuf, size_t destsize)
{
  const uchar *src = *pptr;
  const uchar *send = src + destsize;
  if ( send < src || send > end )
    return NULL;
  *pptr = send;
  return memcpy(destbuf, src, destsize);
}


/// Unpack an object of an unknown size (packed with append_buf()).
/// \param pptr     pointer to packed object
/// \param end      pointer to end of packed object
/// \param[out] sz  size of unpacked object
/// \return pointer to the destination buffer, which is allocated in the dynamic memory.  \n
///         the caller should use qfree() to deallocate it.                               \n
///         if any error, returns NULL.                                                   \n
///         NB: zero size objects will return NULL too.

THREAD_SAFE inline void *idaapi unpack_buf(const uchar **pptr, const uchar *end, size_t *sz)
{
  size_t size = *sz = unpack_dd(pptr, end);
  if ( size == 0 )
    return NULL;
  const uchar *src = *pptr;
  const uchar *srcend = src + size;
  if ( srcend < src || srcend > end )
    return NULL;
  void *dst = qalloc(size);
  if ( dst != NULL )
  {
    memcpy(dst, src, size);
    *pptr = srcend;
  }
  return dst;
}


/// In-place version of unpack_obj().
/// It does not copy any data. It just returns a pointer to the object in the packed string.
/// If any error, it returns NULL.

THREAD_SAFE inline const void *idaapi unpack_obj_inplace(const uchar **pptr, const uchar *end, size_t objsize)
{
  const uchar *ret = *pptr;
  const uchar *rend = ret + objsize;
  if ( rend < ret || rend > end )
    return NULL;
  *pptr = rend;
  return ret;
}


/// In-place version of unpack_buf().
/// It does not copy any data. It just returns a pointer to the object in the packed string.
/// If any error, it returns NULL.
/// \param[out] sz  size of the unpacked string

THREAD_SAFE inline const void *idaapi unpack_buf_inplace(const uchar **pptr, const uchar *end, size_t *sz)
{
  size_t objsize = unpack_dd(pptr, end);
  const uchar *ret = *pptr;
  const uchar *rend = ret + objsize;
  if ( rend < ret || rend > end )
    return NULL;
  *pptr = rend;
  *sz   = objsize;
  return ret;
}


/// Unpack a vector of ea values.
/// \param ea          base value that was used to pack the eavec (see append_eavec())
/// \param[out] insns  resulting vector
/// \param ptr         pointer to packed eavec
/// \param end         pointer to end of packed eavec

THREAD_SAFE inline void unpack_eavec(ea_t ea, eavec_t &insns, const uchar **ptr, const uchar *end)
{
  ea_t old = ea;
  int n = unpack_dw(ptr, end);
  insns.resize(n);
  for ( int i=0; i < n; i++ )
  {
    old += unpack_ea(ptr, end);
    insns[i] = old;
  }
}


/// Unpack an LEB128 encoded (DWARF-3 style) signed/unsigned value.
/// Do not use this function directly - see \ref unp_templates

idaman THREAD_SAFE bool ida_export unpack_xleb128(
        void *res,
        int nbits,
        bool is_signed,
        const uchar **pptr,
        const uchar *end);

/// \defgroup unp_templates Template unpacking
/// Template functions that can unpack values
//@{

template <class T>
inline bool unpack_uleb128(T *res, const uchar **pptr, const uchar *end)
{
  CASSERT((T)(-1) > 0); // make sure T is unsigned
  return unpack_xleb128(res, sizeof(T)*8, false, pptr, end);
}

template <class T>
inline bool unpack_sleb128(T *res, const uchar **pptr, const uchar *end)
{
  CASSERT((T)(-1) < 0); // make sure T is signed
  return unpack_xleb128(res, sizeof(T)*8, true, pptr, end);
}

//@} Template unpacking functions

// packed sizes
/// \cond
static const int ea_packed_size = sizeof(ea_t) + sizeof(ea_t)/4; // 5 or 10 bytes
static const int dq_packed_size = 10;
static const int dd_packed_size = 5;
static const int dw_packed_size = 3;
/// \endcond

inline int ds_packed_size(const char *s) { return s ? int(strlen(s)+dd_packed_size) : 1; }

//----------------------------------------------------------------------------
/// \defgroup pack_vector Vector packing
/// Convenience functions for packing into vectors
//@{


/// Append a byte to a bytevec

THREAD_SAFE inline void append_db(bytevec_t &v, uchar x)
{
  v.push_back(x);
}


/// Append 'size' bytes from 'obj' to the bytevec;

THREAD_SAFE inline void append_obj(bytevec_t &v, const void *obj, size_t size)
{
  v.append(obj, size);
}


/// Pack a word and append the result to the bytevec

THREAD_SAFE inline void append_dw(bytevec_t &v, uint16 x)
{
  uchar packed[3];
  size_t len = pack_dw(packed, packed+sizeof(packed), x) - packed;
  append_obj(v, packed, len);
}


/// Pack a double word and append the result to the bytevec

THREAD_SAFE inline void append_dd(bytevec_t &v, uint32 x)
{
  uchar packed[5];
  size_t len = pack_dd(packed, packed+sizeof(packed), x) - packed;
  append_obj(v, packed, len);
}


/// Pack a quadword and append the result to the bytevec

THREAD_SAFE inline void append_dq(bytevec_t &v, uint64 x)
{
  uchar packed[10];
  size_t len = pack_dq(packed, packed+sizeof(packed), x) - packed;
  append_obj(v, packed, len);
}


/// Pack an ea value and append the result to the bytevec

THREAD_SAFE inline void append_ea(bytevec_t &v, ea_t x)
{
  uchar packed[10];
  size_t len = pack_ea(packed, packed+sizeof(packed), x) - packed;
  append_obj(v, packed, len);
}


/// Pack a string and append the result to the bytevec

THREAD_SAFE inline void append_ds(bytevec_t &v, const char *x)
{
  size_t len = strlen(x);
#ifdef __X64__
  QASSERT(4, len <= 0xFFFFFFFF);
#endif
  append_dd(v, uint32(len));
  append_obj(v, x, len);
}


/// Pack an object of size 'len' and append the result to the bytevec

THREAD_SAFE inline void append_buf(bytevec_t &v, const void *buf, size_t len)
{
#ifdef __X64__
  QASSERT(5, len <= 0xFFFFFFFF);
#endif
  append_dd(v, uint32(len));
  append_obj(v, buf, len);
}


/// Pack an eavec and append the result to the bytevec.
/// Also see unpack_eavec().
/// \param v      output vector
/// \param ea     when we pack an eavec, we only store the differences between each
///               value and this parameter.                                                  \n
///               This is because groups of ea values will likely be similar, and therefore
///               the differences will usually be small.                                     \n
///               A good example is packing the addresses of a function prologue.            \n
///               One can pass the start ea of the function as this parameter,
///               which results in a quick and efficient packing/unpacking.                  \n
///               (Just be sure to use the func's start ea when unpacking, of course)
/// \param insns  eavec to pack

THREAD_SAFE inline void append_eavec(bytevec_t &v, ea_t ea, const eavec_t &insns)
{
  int n = (int)insns.size();
  append_dw(v, (ushort)n);
  ea_t old = ea;
  for ( int i=0; i < n; i++ )
  {
    ea_t nea = insns[i];
    append_ea(v, nea-old);
    old = nea;
  }
}

//@} Convenience functions for packing into vectors


/// Unpack a string in place.
/// \return a pointer to the beginning of the string,
///         and fills 'ptr' with a pointer to the end of the string

THREAD_SAFE inline char *unpack_str(const uchar **ptr, const uchar *end)
{
  char *str = (char *)*ptr;
  *ptr = (const uchar *)strchr(str, '\0') + 1;
  if ( *ptr > end )
    *ptr = end;
  return str;
}

//----------------------------------------------------------------------------
inline int dw_size(uchar first_byte)
{
  return (first_byte & 0x80) == 0    ? 1
       : (first_byte & 0xC0) == 0xC0 ? 3
       :                               2;
}

//----------------------------------------------------------------------------
inline int dd_size(uchar first_byte)
{
  return (first_byte & 0x80) == 0x00 ? 1
       : (first_byte & 0xC0) != 0xC0 ? 2
       : (first_byte & 0xE0) == 0xE0 ? 5
       :                               4;
}

//----------------------------------------------------------------------------
// unpack data from an object which must have the following functions:
//   ssize_t read(void *buf, size_t count)
//   bool eof() - return true if there is no more data to read
template <class T>
inline uchar extract_db(T &v)
{
  uchar x;
  v.read(&x, 1);
  return x;
}

template <class T>
inline void *extract_obj(T &v, void *destbuf, size_t destsize)
{
  if ( destsize == 0 )
    return NULL;
  return v.read(destbuf, destsize) == destsize ? destbuf : NULL;
}

template <class T>
inline uint16 extract_dw(T &v)
{
  uchar packed[3];
  packed[0] = extract_db(v);
  int psize = dw_size(packed[0]);
  extract_obj(v, &packed[1], psize-1);
  const uchar *ptr = packed;
  return unpack_dw(&ptr, packed + psize);
}

template <class T>
inline uint32 extract_dd(T &v)
{
  uchar packed[5];
  packed[0] = extract_db(v);
  int psize = dd_size(packed[0]);
  extract_obj(v, &packed[1], psize-1);
  const uchar *ptr = packed;
  return unpack_dd(&ptr, packed + psize);
}

template <class T>
inline uint64 extract_dq(T &v)
{
  uint32 l = extract_dd(v);
  uint32 h = extract_dd(v);
  return make_ulonglong(l, h);
}

template <class T>
inline ea_t extract_ea(T &v)
{
#ifdef __EA64__
  return extract_dq(v);
#else
  return extract_dd(v);
#endif
}

template <class T>
inline void *extract_buf(T &v, size_t size)
{
  void *buf = qalloc(size);
  if ( buf == NULL )
    return NULL;
  return extract_obj(v, buf, size);
}

template <class T>
inline void *extract_array(T &v, size_t *sz, size_t maxsize)
{
  size_t size = extract_dd(v);
  if ( size == 0 || size > maxsize )
    return NULL;
  *sz = size;
  return extract_buf(v, size);
}

//@} Packing functions

//----------------------------------------------------------------------------

/// Calculate CRC32 (polynom 0xEDB88320, zlib compatible).
/// \note in IDA versions before 6.0 a different, incompatible algorithm was used

idaman THREAD_SAFE uint32 ida_export calc_crc32(uint32 crc, const void *buf, size_t len);


/// Calculate an input source CRC32

idaman THREAD_SAFE uint32 ida_export calc_file_crc32(linput_t *fp);


/// Match a string with a regular expression.
/// \retval 0  no match
/// \retval 1  match
/// \retval -1 error

idaman int ida_export regex_match(const char *str, const char *pattern, bool sense_case);

#endif // SWIG

#ifndef NO_OBSOLETE_FUNCS
typedef idc_value_t value_t;
idaman DEPRECATED THREAD_SAFE char *ida_export addblanks(char *str,ssize_t len);
#endif

//----------------------------------------------------------------------------
/// \defgroup winkeys Compatibility Windows virtual keys
/// compatibility windows virtual keys to use in plugins which are not Qt aware. (check the #CVH_QT_AWARE flag)
/// these keys are provided for compilation of older plugins that use windows virtual keys on all platforms.
/// those constants are currently passed to cli_t->keydown and customview/CVH_KEYDOWN handlers.
//@{
#define IK_CANCEL              0x03
#define IK_BACK                0x08
#define IK_TAB                 0x09
#define IK_CLEAR               0x0C
#define IK_RETURN              0x0D
#define IK_SHIFT               0x10
#define IK_CONTROL             0x11
#define IK_MENU                0x12
#define IK_PAUSE               0x13
#define IK_CAPITAL             0x14
#define IK_KANA                0x15
#define IK_ESCAPE              0x1B
#define IK_MODECHANGE          0x1F
#define IK_SPACE               0x20
#define IK_PRIOR               0x21
#define IK_NEXT                0x22
#define IK_END                 0x23
#define IK_HOME                0x24
#define IK_LEFT                0x25
#define IK_UP                  0x26
#define IK_RIGHT               0x27
#define IK_DOWN                0x28
#define IK_SELECT              0x29
#define IK_PRINT               0x2A
#define IK_EXECUTE             0x2B
#define IK_SNAPSHOT            0x2C
#define IK_INSERT              0x2D
#define IK_DELETE              0x2E
#define IK_HELP                0x2F
#define IK_LWIN                0x5B
#define IK_RWIN                0x5C
#define IK_APPS                0x5D
#define IK_SLEEP               0x5F
#define IK_NUMPAD0             0x60
#define IK_NUMPAD1             0x61
#define IK_NUMPAD2             0x62
#define IK_NUMPAD3             0x63
#define IK_NUMPAD4             0x64
#define IK_NUMPAD5             0x65
#define IK_NUMPAD6             0x66
#define IK_NUMPAD7             0x67
#define IK_NUMPAD8             0x68
#define IK_NUMPAD9             0x69
#define IK_MULTIPLY            0x6A
#define IK_ADD                 0x6B
#define IK_SEPARATOR           0x6C
#define IK_SUBTRACT            0x6D
#define IK_DECIMAL             0x6E
#define IK_DIVIDE              0x6F
#define IK_F1                  0x70
#define IK_F2                  0x71
#define IK_F3                  0x72
#define IK_F4                  0x73
#define IK_F5                  0x74
#define IK_F6                  0x75
#define IK_F7                  0x76
#define IK_F8                  0x77
#define IK_F9                  0x78
#define IK_F10                 0x79
#define IK_F11                 0x7A
#define IK_F12                 0x7B
#define IK_F13                 0x7C
#define IK_F14                 0x7D
#define IK_F15                 0x7E
#define IK_F16                 0x7F
#define IK_F17                 0x80
#define IK_F18                 0x81
#define IK_F19                 0x82
#define IK_F20                 0x83
#define IK_F21                 0x84
#define IK_F22                 0x85
#define IK_F23                 0x86
#define IK_F24                 0x87
#define IK_NUMLOCK             0x90
#define IK_SCROLL              0x91
#define IK_OEM_FJ_MASSHOU      0x93
#define IK_OEM_FJ_TOUROKU      0x94
#define IK_LSHIFT              0xA0
#define IK_RSHIFT              0xA1
#define IK_LCONTROL            0xA2
#define IK_RCONTROL            0xA3
#define IK_LMENU               0xA4
#define IK_RMENU               0xA5
#define IK_BROWSER_BACK        0xA6
#define IK_BROWSER_FORWARD     0xA7
#define IK_BROWSER_REFRESH     0xA8
#define IK_BROWSER_STOP        0xA9
#define IK_BROWSER_SEARCH      0xAA
#define IK_BROWSER_FAVORITES   0xAB
#define IK_BROWSER_HOME        0xAC
#define IK_VOLUME_MUTE         0xAD
#define IK_VOLUME_DOWN         0xAE
#define IK_VOLUME_UP           0xAF
#define IK_MEDIA_NEXT_TRACK    0xB0
#define IK_MEDIA_PREV_TRACK    0xB1
#define IK_MEDIA_STOP          0xB2
#define IK_MEDIA_PLAY_PAUSE    0xB3
#define IK_LAUNCH_MAIL         0xB4
#define IK_LAUNCH_MEDIA_SELECT 0xB5
#define IK_LAUNCH_APP1         0xB6
#define IK_LAUNCH_APP2         0xB7
#define IK_OEM_1               0xBA
#define IK_OEM_PLUS            0xBB
#define IK_OEM_COMMA           0xBC
#define IK_OEM_MINUS           0xBD
#define IK_OEM_PERIOD          0xBE
#define IK_OEM_2               0xBF
#define IK_OEM_3               0xC0
#define IK_OEM_4               0xDB
#define IK_OEM_5               0xDC
#define IK_OEM_6               0xDD
#define IK_OEM_7               0xDE
#define IK_OEM_102             0xE2
#define IK_PLAY                0xFA
#define IK_ZOOM                0xFB
#define IK_OEM_CLEAR           0xFE
//@}

/// Enumeration of form callback special values
enum cb_id
{
  CB_INIT = -1,
  CB_YES  = -2,
  CB_CLOSE = -3
};



#pragma pack(pop)
#endif // __KERNWIN_HPP

/*
*  This plugin demonstrates how to use choosers inside forms.
*
*/

#include <pro.h>
#include <idp.hpp>
#include <loader.hpp>
#include <kernwin.hpp>

//Ponce
#include "formChoser.hpp"
#include "globals.hpp"

void idaapi btn_cb(TView *[], int)
{
	warning("button pressed");
}

//--------------------------------------------------------------------------
int idaapi modcb(int fid, form_actions_t &fa)
{
	ushort isActivated;
	fa.get_checkbox_value(fid, &isActivated);
	
	msg("fid is %d and it is: %d\n", fid, isActivated);
	switch (fid)
	{
	case -1:
		msg("initializing\n");
		break;
	case -2:
		msg("terminating\n");
		break;
	case 10:     // show debug info
		cmdOptions.showDebugInfo = isActivated ? 1 : 0;
		break;
	case 11:     // manage symbolic indexing
		cmdOptions.manageSymbolicIndexing = isActivated ? 1 : 0;
		break;
	case 12:     // enable tracing first time something is tainted
		cmdOptions.enableTracingAsTainted = isActivated ? 1 : 0;
		break;
	case 13:     // Limit the number of instructions in tracing mode
		/*if (isActivated)
			fa.enable_field(4, true);*/
		cmdOptions.limitInstructionsTracingMode = isActivated ? 1 : 0;
		break;
	case 14:     // Limit of instructions to execute before ask to the user
		cmdOptions.limitInstructionsBeforeAskingUser = isActivated ? 1 : 0;
		break;
	case 15:     // Time limit before ask user
		cmdOptions.limitTime = isActivated ? 1 : 0;
		break;
	case 16:     // Only use tainting mode
		cmdOptions.onlyTainting = isActivated ? 1 : 0;
		break;
	case 17:     // Automatically rename the functions with tainted instructions/conditions
		cmdOptions.RenameFunctionNames = isActivated ? 1 : 0;
		break;
	case 18:     // Configure automated tainting
		cmdOptions.automatedTainting = isActivated ? 1 : 0;
		break;
	case 19:     // Taint argv[0]
		cmdOptions.taintArgv0 = isActivated ? 1 : 0;
		break;
	case 20:     // Taint argc
		cmdOptions.taintArgc = isActivated ? 1 : 0;
		break;
	default:
		msg("unknown id %d\n", fid);
		break;
	}
	return 1;
	//bool is_gui = callui(ui_get_hwnd).vptr != NULL || is_idaq();

	//char buf0[MAXSTR];
	//if (!fa.get_ascii_value(5, buf0, sizeof(buf0)))
	//	INTERR(30145);

	//if (streq(buf0, "on"))
	//	fa.enable_field(12, true);

	//if (streq(buf0, "off"))
	//	fa.enable_field(12, false);

	//ushort buf1;
	//if (!fa.get_cbgroup_value(12, &buf1))
	//	INTERR(30146);

	//fa.show_field(7, (buf1 & 1) != 0);
	//fa.enable_field(8, (buf1 & 2) != 0);


	//ushort c13;
	//if (!fa.get_checkbox_value(13, &c13))
	//	INTERR(30147);
	//fa.enable_field(10, c13 != 0);

	//ushort c14;
	//if (!fa.get_checkbox_value(14, &c14))
	//	INTERR(30148);
	//fa.enable_field(5, c14 != 0);

	//ushort c15;
	//if (!fa.get_checkbox_value(15, &c15))
	//	INTERR(30149);

	//if ((buf1 & 8) != 0)
	//{
	//	sval_t x, y, w, h;
	//	fa.get_signed_value(4, &x);
	//	fa.get_signed_value(3, &y);
	//	fa.get_signed_value(2, &w);
	//	fa.get_signed_value(1, &h);
	//	fa.move_field(5, x, y, w, h);
	//	if (x != -1 && c15)
	//		fa.move_field(-5, x - 7, y, w, h);
	//}

	//// get_field_value() for buttons must return false always
	//if (fa._get_field_value(7, NULL))
	//	INTERR(30150);

	//bgcolor_t bgc = -1;
	//if (is_gui && !fa.get_color_value(8, &bgc))
	//	INTERR(30151);
	//msg("  op=%s change=%x color=%x\n", buf0, buf1, bgc);

	//fa.set_label_value(9, buf0);
	//return 1;
}


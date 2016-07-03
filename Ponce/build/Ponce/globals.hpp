#pragma once


extern int g_nb_insn;
extern const int g_max_insn;
extern bool hooked;

struct action{
	char* name;
	char* label;
	const action_desc_t* callback;
	int view_type[44]; // This are the posible views
};

extern struct action action_list[];


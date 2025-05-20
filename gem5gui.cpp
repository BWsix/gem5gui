#include "beamlib/blib.h"
#include "imgui.h"

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define TEXT_INPUT_SIZE 512

char gem5_root_dir[TEXT_INPUT_SIZE] = "/home/USER/PATH/TO/GEM5/ROOT/DIR";
char gem5_program[TEXT_INPUT_SIZE] = "./build/RISCV/gem5.opt";
char trace_file_name[TEXT_INPUT_SIZE] = "trace.out";
char program_name[TEXT_INPUT_SIZE] = "/home/USER/PATH/TO/YOUR/PROGRAM";

typedef struct {
	char content[TEXT_INPUT_SIZE];
	bool enabled;
} flag_t;

#define FLAG_COUNT 10
flag_t flags[FLAG_COUNT] = {0};

void render_flag(flag_t* flag, size_t index)
{
	char buf[16] = {0};

	sprintf(buf, "##flab_cb%zu", index);
        ImGui::Checkbox(buf, &(flag->enabled));
	ImGui::SameLine();

	sprintf(buf, "flag %zu", index);
	ImGui::InputText(buf, &(flag->content[0]), TEXT_INPUT_SIZE);
}

void set_flag(size_t idx, const char *content)
{
	flags[idx].enabled = content;
	if (content) {
		strcpy(&(flags[idx].content[0]), content);
	}
}

int main()
{
	for (size_t i = 0; i < FLAG_COUNT; i++) {
		set_flag(i, NULL);
	}
	set_flag(0, "system.cpu[0].dispatchWidth=1");

	GLFWwindow *const window = Blib::CreateWindow("gem5gui", 800, 600);
	while (!Blib::WindowShouldClose(window)) {
		Blib::BeginUI();
		Blib::BeginFullscreenWindow("Fullscreen window", NULL);

		ImGui::SeparatorText("Configs");
		ImGui::InputText("gem5_root_dir", gem5_root_dir, TEXT_INPUT_SIZE);
		ImGui::InputText("gem5_program", gem5_program, TEXT_INPUT_SIZE);
		ImGui::InputText("trace_file_name", trace_file_name, TEXT_INPUT_SIZE);
		ImGui::InputText("program_name", program_name, TEXT_INPUT_SIZE);

		ImGui::SeparatorText("Flags");
		for (size_t i = 0; i < FLAG_COUNT; i++) {
			render_flag(&(flags[i]), i);
		}

		char command[2048] = {0};
		size_t command_sz = sprintf(command, "cd %s && %s --debug-flags=O3PipeView,O3CPUAll --debug-file=%s configs/deprecated/example/se.py --cpu-type=RiscvO3CPU --caches -c %s", gem5_root_dir, gem5_program, trace_file_name, program_name);
		for (size_t i = 0; i < FLAG_COUNT; i++) {
			if (flags[i].enabled) {
				command_sz += sprintf(command + command_sz, " -P \"%s\"", flags[i].content);
			}
		}

		ImGui::SeparatorText("");
		ImGui::TextWrapped("gem5gui will run the following command using system():\n%s", command);
		if (ImGui::Button("Generate trace file!")) {
			system(command);
		}

		Blib::EndFullscreenWindow();
		Blib::EndUI();
	}
	Blib::DestroyWindow(window);
	return 0;
}

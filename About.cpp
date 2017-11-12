#include "About.h"
#include "Application.h"
#include "imgui.h"

About::About() : Panel("About", SDL_SCANCODE_UNKNOWN, false)
{
}

About::~About()
{
}

void About::Start()
{
}

void About::Draw()
{
	if (!Active)
		return;

	if (ImGui::Begin("About RoSer Engine", &Active, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("RoSer Engine %s", App->GetVersion());
		ImGui::Separator();
		ImGui::Text("3D engine developed as a project in Game Design and Game Development degree at CITM.");
		ImGui::Text("Developed by Roger Busquets Duran and Sergi Parra Ramirez.");
		if (ImGui::Button("Github Repository"))
			App->RequestBrowser("https://github.com/rogerbusquets97/3D-Engine");

		ImGui::SameLine();
		if (ImGui::Button("Download Latest Release"))
			App->RequestBrowser("https://github.com/rogerbusquets97/3D-Engine/releases");

		ImGui::SameLine();
		if (ImGui::Button("Report Issue"))
			App->RequestBrowser("https://github.com/rogerbusquets97/3D-Engine/issues");
	
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Libraries")){
			ImGui::Columns(2, "Name");
			ImGui::Separator();
			ImGui::Text("Name"); ImGui::NextColumn();
			ImGui::Text("Version"); ImGui::NextColumn();
			ImGui::Separator();
			const char* name[10] = { "SDL", "MathGeoLib", "ImGui", "ImGuizmo", "Parson", "OpenGL", "Glew", "Devil", "Assimp", "mmgr" };
			const char* version[10] = { "v2.0.4", "v1.5", "v1.52", "Last version", "2017 version", "v2.1", "v2.0", "v1.7.8", "", "" };
			static int selected = -1;
			for (int i = 0; i < 10; i++)
			{
				ImGui::Text(name[i]); ImGui::NextColumn();
				ImGui::Text(version[i]); ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::Separator();
		}
		if (ImGui::CollapsingHeader("License"))	{
			ImGui::Text("MIT License");
			ImGui::Text("Copyright(c) 2017 Roger Busquets Duran and Sergi Parra Ramirez");
			ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
			ImGui::Text("of this software and associated documentation files(the 'Software'), to deal");
			ImGui::Text("in the Software without restriction, including without limitation the rights");
			ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and / or sell");
			ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
			ImGui::Text("furnished to do so, subject to the following conditions :");

			ImGui::TextColored({ 1, 0.2f, 0.2f, 1 }, "The above copyright notice and this permission notice shall be included in all");
			ImGui::TextColored({ 1, 0.2f, 0.2f, 1 }, "copies or substantial portions of the Software.");

			ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
			ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
			ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
			ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
			ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
			ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
			ImGui::Text("SOFTWARE.");
		}

	}
	ImGui::End();
}

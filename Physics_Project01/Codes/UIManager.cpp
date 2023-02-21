#include "UIManager.h"
#include "..\imgui\imgui_impl_glfw.h"
#include "..\imgui\imgui_impl_opengl3.h"
#include "OpenGLDevice.h"
#include "Define.h"
#include "SceneDungeon.h"

#include <sstream>
#include <iomanip>

SINGLETON_FUNCTION(UIManager)
USING(Engine)
USING(ImGui)
USING(std)

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

// Call instead of destructor to manage class internal data
void UIManager::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	DestroyContext();
}

// Basic Render Function
void UIManager::RenderUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	NewFrame();

	ImVec2 screen = ImVec2((_float)COpenGLDevice::GetInstance()->GetWidthSize(), (_float)COpenGLDevice::GetInstance()->GetHeightSize());
	SetNextWindowPos(ImVec2(0.f, 0.f));
	SetNextWindowSize(screen);
	if (Begin("UI", (bool*)0,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar))
	{
		Text("Tab : Next Target");
		Text("F1 : Remove Target");
		Text(" ");
		Text("WASD / Space : Move Target");
		Text("F2 : Reset all objects");
		Text("F3 : Apply random force to all objects");
		Text(" ");
		Text("Move Mouse : Rotate Camera");
		Text("Scroll Mouse Wheel : Zoom In/Out");
		Text("ESC : Quit");
		Text(" ");
		if (nullptr != m_pScene)
		{
			Text(" * Current Target : "); SameLine(150.f);
			string name = m_pScene->GetCurrentTargetName();;

			PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
			Text(name.c_str());
			PopStyleColor();
		}
	}
	End();

	Render();
	ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}

// Initialize
RESULT UIManager::Ready(SceneDungeon* pScene)
{
	m_pScene = pScene;

	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = GetIO();

	stringstream filePath;
	filePath << pScene->GetDataPath() << "Font\\OpenSans-SemiBold.ttf";
	io.Fonts->AddFontFromFileTTF(filePath.str().c_str(), 17.f);
	io.FontDefault = io.Fonts->Fonts[0];

	if (!ImGui_ImplGlfw_InitForOpenGL(COpenGLDevice::GetInstance()->GetWindow(), true) ||
		!ImGui_ImplOpenGL3_Init("#version 460"))
		return PK_ERROR_IMGUI;
	StyleColorsDark();

	return PK_NOERROR;
}
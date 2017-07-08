#include "CCImGuiLayer.h"
#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "CCIMGUI.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool ImGuiLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	// init imgui
	CCIMGUI::getInstance()->setWindow(((GLViewImpl*)Director::getInstance()->getOpenGLView())->getWindow());
    setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));

	// TODO: Fix the mouse callbacks. Don't use TouchOneByOne
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event*) -> bool {
        bool inImGuiWidgets = ImGui::IsPosHoveringAnyWindow(ImVec2(touch->getLocationInView().x, touch->getLocationInView().y));
        //CCLOG("touch in ImGui widgets %s", inImGuiWidgets ? "yes" : "no");
        return inImGuiWidgets;
    };

	// TODO: Add the keyboard callbacks here:


    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


    return true;
}

void ImGuiLayer::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(ImGuiLayer::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void ImGuiLayer::onDraw()
{
    glUseProgram(0);
    if (CCIMGUI::getInstance()->getWindow()) {
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = Director::getInstance()->getDeltaTime();

        // create frame
        ImGui_ImplGlfw_NewFrame();

        // draw all gui
		CCIMGUI::getInstance()->updateImGUI();

        // rendering
        ImGui::Render();
    }
	glUseProgram(1);
}

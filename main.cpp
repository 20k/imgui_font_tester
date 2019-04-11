#include <iostream>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <imgui/misc/freetype/imgui_freetype.h>

int main()
{
    sf::ContextSettings sett;
    sett.antialiasingLevel = 8;
    sett.sRgbCapable = false;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "hi", sf::Style::Default, sett);

    sf::Texture font_atlas;

    ImGui::SFML::Init(window, false);

    ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    ImFont* font = io.Fonts->AddFontFromFileTTF("VeraMono.ttf", 14.f);

    ImFontAtlas* atlas = ImGui::SFML::GetFontAtlas();

    auto write_data =  [](unsigned char* data, void* tex_id, int width, int height)
    {
        sf::Texture* tex = (sf::Texture*)tex_id;

        tex->create(width, height);
        tex->update((const unsigned char*)data);
    };

    ImGuiFreeType::BuildFontAtlas(atlas, ImGuiFreeType::ForceAutoHint, ImGuiFreeType::LEGACY, write_data, (void*)&font_atlas);

    atlas->TexID = (void*)font_atlas.getNativeHandle();

    ImGui::SetStyleLinearColor(sett.sRgbCapable);

    sf::Clock imgui_delta;

    while(window.isOpen())
    {
        sf::Event event;

        while(window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if(event.type == sf::Event::Closed)
            {
                window.close();
            }

            if(event.type == sf::Event::Resized)
            {
                window.create(sf::VideoMode(event.size.width, event.size.height), "hi", sf::Style::Default, sett);
            }
        }

        ImGui::SFML::Update(window,  imgui_delta.restart());

        ImGui::Begin("Font Tester");

        ImGui::Text("Hello font 1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

        ImGui::Text("The quick brown fox jumps over the lazy dog ({}).!<>/|\\");

        ImGui::End();

        ImGui::SFML::Render(window);

        window.display();
        window.clear();

        sf::sleep(sf::milliseconds(4));
    }
}

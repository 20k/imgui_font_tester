#include <iostream>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <imgui/misc/freetype/imgui_freetype.h>

bool CheckBoxHelper(uint32_t& mode, int target_mode, const std::string& text)
{
    bool is_mode = mode == target_mode;
    bool was_mode = is_mode;

    ImGui::Checkbox(text.c_str(), &is_mode);

    if(is_mode)
    {
        mode = target_mode;
    }

    return is_mode != was_mode;
}

int main()
{
    sf::ContextSettings sett;
    sett.antialiasingLevel = 8;
    sett.sRgbCapable = false;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "hi", sf::Style::Default, sett);

    sf::Texture font_atlas;

    ImGui::SFML::Init(window, false);

    bool rebuild_font = true;
    uint32_t subpixel_mode = ImGuiFreeType::LEGACY;

    ImGui::SetStyleLinearColor(sett.sRgbCapable);

    sf::Clock imgui_delta;

    while(window.isOpen())
    {
        {
            if(rebuild_font)
            {
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

                uint32_t font_mode = ImGuiFreeType::ForceAutoHint;

                ImGuiFreeType::BuildFontAtlas(atlas, font_mode, subpixel_mode);

                write_data((unsigned char*)atlas->TexPixelsNewRGBA32, (void*)&font_atlas, atlas->TexWidth, atlas->TexHeight);

                atlas->TexID = (void*)font_atlas.getNativeHandle();

                rebuild_font = false;
            }
        }

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

        rebuild_font |= CheckBoxHelper(subpixel_mode, ImGuiFreeType::DEFAULT, "DEFAULT");
        rebuild_font |= CheckBoxHelper(subpixel_mode, ImGuiFreeType::LEGACY, "LEGACY");
        rebuild_font |= CheckBoxHelper(subpixel_mode, ImGuiFreeType::LIGHT, "LIGHT");
        rebuild_font |= CheckBoxHelper(subpixel_mode, ImGuiFreeType::NONE, "NONE");
        rebuild_font |= CheckBoxHelper(subpixel_mode, ImGuiFreeType::DISABLE_SUBPIXEL_AA, "DISABLE_SUBPIXEL_AA");

        ImGui::End();

        ImGui::SFML::Render(window);

        window.display();
        window.clear();

        sf::sleep(sf::milliseconds(4));
    }
}

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

    uint32_t use_stb = 0;

    ImGui::SetStyleLinearColor(sett.sRgbCapable);

    sf::Clock imgui_delta;

    while(window.isOpen())
    {
        {
            if(rebuild_font)
            {
                ImGuiIO& io = ImGui::GetIO();
                io.Fonts->Clear();

                //io.Fonts->AddFontDefault();
                ImFont* font = io.Fonts->AddFontFromFileTTF("VeraMono.ttf", 14.f);

                if(use_stb)
                {
                    ImFontAtlas* natlas = new ImFontAtlas;

                    ImGui::CreateContext(natlas);

                    ImGuiIO& io = ImGui::GetIO();
                    unsigned char* pixels;
                    int width, height;

                    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

                    font_atlas.create(width, height);
                    font_atlas.update(pixels);

                    io.Fonts->TexID = (void*)font_atlas.getNativeHandle();
                }
                else
                {

                    ImFontAtlas* atlas = ImGui::SFML::GetFontAtlas();

                    uint32_t font_mode = ImGuiFreeType::ForceAutoHint;

                    ImGuiFreeType::BuildFontAtlas(atlas, font_mode, subpixel_mode);

                    font_atlas.create(atlas->TexWidth, atlas->TexHeight);
                    font_atlas.update((const unsigned char*)atlas->TexPixelsNewRGBA32);

                    atlas->TexID = (void*)font_atlas.getNativeHandle();

                    rebuild_font = false;
                }
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

        ImGui::NewLine();

        rebuild_font |= CheckBoxHelper(use_stb, 0, "Freetype");
        rebuild_font |= CheckBoxHelper(use_stb, 1, "STB");

        ImGui::End();

        ImGui::SFML::Render(window);

        window.display();
        window.clear();

        sf::sleep(sf::milliseconds(4));
    }
}

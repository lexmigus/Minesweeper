#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "board.hpp"

void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main() {
    // Initialize GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return -1;

    // Create a window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Window", NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Initialize ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410 core");
    
    int x_coords = 9;
    int y_coords = 9;
    int place_holder_x_coords = 9;
    int place_holder_y_coords = 9;
    int number_of_mines = 10;
    int number_of_mines_placeholder = 10;
    bool is_lost = false;
    bool new_game = false;
    bool toggle_flag = false;

    Board board(x_coords, y_coords, number_of_mines);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)mode->width, (float)mode->height));
        ImGui::Begin("FullScreenWindow", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
                
        ImGui::Text("Enter x: ");
        ImGui::SameLine();
        ImGui::InputInt("##x_coords", &place_holder_x_coords);
        ImGui::Text("Enter y: ");
        ImGui::SameLine();
        ImGui::InputInt("##y_coords", &place_holder_y_coords);
        ImGui::SameLine();
        ImGui::InputInt("##number_of_mines", &number_of_mines);

        if (ImGui::Button("New Game")) {
            new_game = true;
        }
        
        if (new_game) {
            if (place_holder_x_coords > 0 && place_holder_y_coords > 0) {
                x_coords = place_holder_x_coords;
                y_coords = place_holder_y_coords;
                number_of_mines = number_of_mines_placeholder;
                board = Board(place_holder_x_coords, place_holder_y_coords, number_of_mines);
                board.placeMines(number_of_mines, 0, 0);
                new_game = false;
                is_lost = false;
            } else {
                fprintf(stderr, "Invalid board size\n");
            }
        }
        
        // Render the board
        if (x_coords > 0 && y_coords > 0) {
            for (int j = 0; j < y_coords; j++) {
                for (int i = 0; i < x_coords; i++) {
                    std::string button_id = "##button_" + std::to_string(i) + "_" + std::to_string(j);
                    if (board.isFlagged(i, j)) {
                        if (ImGui::Button(("F" + button_id).c_str())) {
                            board.toggleFlag(i, j);
                        }
                    } else if (!board.isRevealed(i, j)) {
                        if(toggle_flag) {
                            if (ImGui::Button(("-" + button_id).c_str())) {
                                board.toggleFlag(i, j);
                            }
                        } else {
                            if (ImGui::Button(("-" + button_id).c_str())) {
                                if(board.revealCell(i, j)) {
                                    is_lost = true;
                                    for (int l = 0; l < y_coords; l++) {
                                        for (int k = 0; k < x_coords; k++) {
                                            board.revealCell(k,l);
                                        }
                                    }
                                }
                            }
                        }
                    } else if (board.surroundingMines(i, j) == 0 && !board.isBomb(i, j)) {
                        if (ImGui::Button(("." + button_id).c_str())) {
                            board.revealCell(i, j);
                        }
                    } else if(board.isBomb(i,j)) {
                            ImGui::Button(("X" + button_id).c_str());
                    } else {
                        ImGui::Button((std::to_string(board.surroundingMines(i, j)) + button_id).c_str());
                    }
                    if (i < x_coords - 1) {
                        ImGui::SameLine();
                    }
                }
            }
        }
        
        ImGui::Checkbox("Toggle Flag", &toggle_flag);
        
        ImGui::Text("x: %d y: %d", x_coords, y_coords);
        
        if(is_lost) {
            ImGui::Text("Game Over");
        } else if(board.hasWon()) {
            ImGui::Text("You Win!");
        } else {
            ImGui::Text("Game in progress");
        }

        ImGui::End();

        // Render
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    
    // Closing used stuff
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}

// g++ -std=c++17 \
    -DGL_SILENCE_DEPRECATION \
    -Iimgui -Iimgui/backends -I/opt/homebrew/include \
    -L/opt/homebrew/lib -lglfw \
    -framework OpenGL \
    main.cpp imgui/*.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp \
    -o Minesweeper

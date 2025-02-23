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
    
    // Initial values and placeholders for board
    int x_coords = 9;
    int y_coords = 9;
    int place_holder_x_coords = 9;
    int place_holder_y_coords = 9;
    int number_of_mines = 10;
    int number_of_mines_placeholder = 10;
    bool is_lost = false;
    bool new_game = false;
    bool toggle_flag = false;
    bool first_move = true;

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
        
        // User input
        ImGui::Text("Enter x: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputInt("##x_coords", &place_holder_x_coords);
        ImGui::NewLine();
        ImGui::Text("Enter y: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputInt("##y_coords", &place_holder_y_coords);
        ImGui::NewLine();
        ImGui::Text("Mines:   ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputInt("##number_of_mines", &number_of_mines_placeholder);
        
        // New game button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));  // Green colour
        if (ImGui::Button("New Game")) {
            new_game = true;
        }
        ImGui::PopStyleColor();
        
        // Popups for win and lose
        if(is_lost) {
            ImGui::OpenPopup("GameOver");
        } else if(board.hasWon()) {
            ImGui::OpenPopup("Win");
        }
        
        if (ImGui::BeginPopup("GameOver")) {
            ImGui::Text("You lost!");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));
            if (ImGui::Button("New Game")) {
                is_lost = false;
                new_game = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor();
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("Win")) {
            ImGui::Text("You won!");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));
            if (ImGui::Button("New Game")) {
                new_game = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor();
            ImGui::EndPopup();
        }
        
        // Initializes board and resets values for new game
        if (new_game) {
            if (place_holder_x_coords > 0 && place_holder_y_coords > 0 && number_of_mines_placeholder < (place_holder_x_coords * place_holder_y_coords)) {
                x_coords = place_holder_x_coords;
                y_coords = place_holder_y_coords;
                number_of_mines = number_of_mines_placeholder;
                board = Board(place_holder_x_coords, place_holder_y_coords, number_of_mines);
                first_move = true;
                new_game = false;
                is_lost = false;
            } else {
                ImGui::Text("Invalid board size or number of mines");
            }
        }
        
        // Render the board
        if (x_coords > 0 && y_coords > 0) {
            for (int j = 0; j < y_coords; j++) {
                for (int i = 0; i < x_coords; i++) {
                    std::string button_id = "##button_" + std::to_string(i) + "_" + std::to_string(j);
                    if (board.isFlagged(i, j)) {
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.2f, 0.6f, 1.0f));
                        if (ImGui::Button((" " + button_id).c_str())) {
                            board.toggleFlag(i, j);
                        }
                        ImGui::PopStyleColor();
                    } else if (!board.isRevealed(i, j)) {
                        if(toggle_flag) {
                            if (ImGui::Button((" " + button_id).c_str())) {
                                board.toggleFlag(i, j);
                            }
                        } else {
                            if (ImGui::Button((" " + button_id).c_str())) {
                                if(first_move) {
                                    board.placeMines(number_of_mines, i, j);
                                    first_move = false;
                                }
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
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));  // Red colour 
                        if (ImGui::Button((" " + button_id).c_str())) {
                            board.revealCell(i, j);
                        }
                        ImGui::PopStyleColor();
                    } else if(board.isBomb(i,j)) {
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // Red colour
                            ImGui::Button((" " + button_id).c_str());
                            ImGui::PopStyleColor();
                    } else {
                        ImGui::Button((std::to_string(board.surroundingMines(i, j)) + button_id).c_str());
                    }
                    if (i < x_coords - 1) {
                        ImGui::SameLine();
                    }
                }
            }
        }
        // Checkbox to toggle flag
        ImGui::Checkbox("Toggle Flag", &toggle_flag);
        
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

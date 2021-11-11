#include <Game/GameScene.hpp>
#include <Game/Actions/JumpAction.hpp>
#include <Game/Actions/CameraRotateAction.hpp>
#include <Game/Actions/AttackAction.hpp>

#include <Engine/Graphics/ShaderProgram.hpp>

GameScene::GameScene(Engine& engine)
	: Scene(engine)
{
	input.registerKeyboardAction(KeyCode::Space, std::unique_ptr<ButtonAction>(new JumpAction));
	input.registerMouseMoveAction(std::unique_ptr<CameraRotateAction>(new CameraRotateAction));
	input.registerMouseButtonAction(MouseButton::Left, std::unique_ptr<AttackAction>(new AttackAction));

	Shader vertexShader("../../../src/Game/shader.vert", ShaderType::Vertex);
	Shader fragmentShader("../../../src/Game/shader.frag", ShaderType::Fragment);

	ShaderProgram shader({ vertexShader, fragmentShader });

	shader.use();

    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}



void GameScene::update()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
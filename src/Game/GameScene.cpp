#include <Game/GameScene.hpp>

#include <Engine/Graphics/ShaderProgram.hpp>

GameScene::GameScene(Engine& engine)
	: Scene(engine)
    , shader([]{
        Shader vertexShader("../../../src/Game/shader.vert", ShaderType::Vertex);
        Shader fragmentShader("../../../src/Game/shader.frag", ShaderType::Fragment);
        ShaderProgram s;
        s.addShader(vertexShader);
        s.addShader(fragmentShader);
        s.link();
        return s;
    }())
{
    input.registerKeyboardButton("jump", KeyCode::Space);
    input.registerMouseButton("attack", MouseButton::Left);

    //s.use();
    //s.setVec2("offset", Vec2(.5, 0));
    //shader = &s;

    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

#include <iostream>

void GameScene::update()
{
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    glClearColor(0.2f, greenValue, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

    if (input.isButtonDown("jump"))
    {
        std::cout << "Jump down\n";
        //shader->setVec3("color", Vec3(float(rand() % 100) / 100, 1.0, 0.0));
    }
    if (input.isButtonHeld("jump"))
    {
        std::cout << "Jump held\n";
    }
    if (input.isButtonUp("jump"))
    {
        std::cout << "Jump up\n";
    }

    if (input.isButtonDown("attack"))
    {
        std::cout << "Attack down\n";
    }
    if (input.isButtonHeld("attack"))
    {
        std::cout << "Attack held\n";
    }
    if (input.isButtonUp("attack"))
    {
        std::cout << "Attack up\n";
    }

    // draw our first triangle
    glUseProgram(shader.handle());
    shader.setVec3("color", Vec3(0.0, greenValue, 0.0));
    shader.setVec2("offset", input.mousePos());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
//#include <Engine/Debug/Drawing.hpp>
//#include <Engine/Graphics/ShaderProgram.hpp>
//#include <Engine/Graphics/VertexArray.hpp>
//#include <Engine/Graphics/VertexBuffer.hpp>

//void Debug::drawLine(const Vec3& start, const Vec3& end, const Vec3& color)
//{
//	static Vec3 data[2] = { Vec3(0, 0, 0), Vec3(0, 0, 1) };
//	static Gfx::ShaderProgram shader = [](){
//		Gfx::Shader vertexShader("src/Engine/Debug/line.vert", Gfx::ShaderType::Vertex);
//		Gfx::Shader fragmentShader("src/Engine/Debug/line.frag", Gfx::ShaderType::Fragment);
//		Gfx::ShaderProgram s;
//		s.addShader(vertexShader);
//		s.addShader(fragmentShader);
//		s.link();
//		return s;
//	}();
//	static Gfx::VertexArray vao;
//	static Gfx::VertexBuffer vbo = []() {
//		vao.bind();
//		Gfx::VertexBuffer vbo(data, 2 * sizeof(Vec3));
//		vbo.bind();
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), reinterpret_cast<void*>(0));
//		glEnableVertexAttribArray(0);
//		return vbo;
//	}();
//
//	data[0] = start;
//	data[1] = end;
//
//	vao.bind();
//	vbo.bind();
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vec3) * 2, data);
//
//	shader.setVec3("color", color);
//	shader.use();
//
//	glDrawArrays(GL_LINES, 0, 2);
//}
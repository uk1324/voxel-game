#include <Engine/Engine.hpp>
#include <Game/GameScene.hpp>

// TODO: Move window creation and initialization out of engine class

// TODO: Check if direction are programmed correctly by using a 3D model of axis.

// TODO: Add collider classes

// TODO: Maybe create reusable classes for non copyable resources

// Making Position use a Vec4 might allow for simd and won't require any extra space

#include <Math/Quaternion.hpp>
#include <Math/Angles.hpp>

#include <Lang/Debug/Disassembler.hpp>

#include <iostream>

#include <iomanip>

int main()
{
	Lang::ByteCode code;
	code.emitOp(Lang::Op::Add);
	code.emitOp(Lang::Op::Add);
	code.emitOp(Lang::Op::Add);
	code.emitOp(Lang::Op::Return);
	code.emitOp(Lang::Op::Return);
	code.emitOp(Lang::Op::Return);
	code.emitOp(Lang::Op::Return);
	code.emitOp(Lang::Op::Return);
	code.emitOp(Lang::Op::Return);
	code.emitOp(Lang::Op::Add);
	code.emitOp(Lang::Op::Add);
	code.emitOp(Lang::Op::Add);
	code.emitOp(Lang::Op::Add);
	code.emitOp(Lang::Op::Add);
	code.emitOp(Lang::Op::Add);
	Lang::disassembleByteCode(code);

	return 0;
	Engine engine(60, 800, 600, "game");

	engine.addScene("game", std::unique_ptr<Scene>(new GameScene(engine)));

	engine.run("game");	
}
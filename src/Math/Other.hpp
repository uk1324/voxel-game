#pragma once

template<typename T>
T signNonZero(T x)
{
	return (x < 0) ? -1 : 1;
}
#pragma once

template <class QtClass_T>
QByteArray toLatin1(const QtClass_T& obj)
{
	return obj.toLatin1();
}

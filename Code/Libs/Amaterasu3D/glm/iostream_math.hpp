#include <glm/glm.hpp>
#include <iostream>

#ifndef glm_iostream_math
#define glm_iostream_math

/*
 * Vec4
 */
template< class T >
inline std::ostream & operator << (std::ostream & s, const glm::detail::tvec4<T> & v)
{
	s << "<" << v.x << "," << v.y << "," << v.z << "," << v.w << ">";
	return s;
}

/*
 * Vec2
 */
template< class T >
inline std::ostream& operator<< (std::ostream & s, const glm::detail::tvec2<T>& v)
{
	s << "<" << v.x << "," << v.y << ">";
	return s;
}

template< class T >
inline std::istream& operator>> (std::istream & s, glm::detail::tvec2<T>& v)
{
	T x,y;
	s >> x >> y;
	v.x = x;
	v.y = y;
	return s;
}

#endif

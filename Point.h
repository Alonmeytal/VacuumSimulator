/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#ifndef POINT_H_
#define POINT_H_

class Point {
	int x, y;
public:
	Point(int _x, int _y): x(_x), y(_y) {}
	Point(const Point& p) : x(p.getX()), y(p.getY()) {}
	Point& operator=(const Point& p) { x = p.getX(); y = p.getY(); return *this;}
	Point distance(const Point& other)const { return Point(x - other.x, y -other.y); }
	int getX() const { return x; }
	int getY() const { return y; }
	void up() { y++; }
	void down() { y--; }
	void right() { x++; }
	void left() { x--; }
	~Point() = default;
};

#endif /* POINT_H_ */

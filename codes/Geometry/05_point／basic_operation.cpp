point operator+(point b) {return {x+b.x, y+b.y}; }
point operator-(point b) {return {x-b.x, y-b.y}; }
point operator*(T b) {return {x*b, y*b}; }
point operator/(T b) {return {x/b, y/b}; }
bool operator==(point b) {return x==b.x && y==b.y; }

T operator*(point b) {return x * b.x + y * b.y; }
T operator^(point b) {return x * b.y - y * b.x; }


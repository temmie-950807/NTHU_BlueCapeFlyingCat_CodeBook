// 旋轉 Arg(b) 的角度（小心溢位）
point rotate(point b){return {x*b.x-y*b.y, x*b.y+y*b.x};}
// 回傳極座標角度，值域：[-π, +π]
friend ld Arg(point b) {
    return (b.x != 0 || b.y != 0) ? atan2(b.y, b.x) : 0;
}
friend T abs2(point b) {return b * b; }

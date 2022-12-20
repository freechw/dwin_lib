// https://blog.csdn.net/wangzibigan/article/details/86003616

#include <Arduino.h>

typedef struct pointType
{
  u16 x;
  u16 y;
} pointType; /**< xy position of control */

//设置前景色与背景色
void ui_SetFCBC(u16 fc, u16 bc);

//取某点颜色到前景色
void ui_GetPointToFC(u16 x, u16 y);

//取某点颜色到背景色
void ui_GetPointToBC(u16 x, u16 y);

//设置字符间距和行间距
void ui_SetInterval(u8 dx, u8 dy);

//设置显示光标
// T5UIC2不支持本命令！！！
void ui_SetShowCursor(u16 x, u16 y, u8 dx, u8 dy, u8 en);

//使用背景色清屏
void ui_Clear(void);

//画点
// mod:0 恢复点背景色 ，1 前景色设置点
void ui_DrawPointMulti(u8 mod, pointType po[], u8 num);

//连线
// mod:0  背景色 ，1  前景色
void ui_DrawLineMulti(u8 mod, pointType po[], u8 num);

//画频谱
void ui_DrawSpectrogram(u16 x, u16 y, u16 maxH, u8 arr[], u8 num);

//固定X间隔的折线图
void ui_Polyline(u16 x, u16 dx, u8 arr[], u8 num);

//画圆
// mod:0圆线反色，1正常画圆线，2圆区域反色，3正常实心圆
// T5UIC2只支持mod=1,3 !!!
void ui_DrawCircle(u16 x, u16 y, u8 r, u8 mod);

//多个同心圆
void ui_DrawCircleConcentric(u16 x, u16 y, u8 r, u8 mod, u8 dr, u8 num);

//画圆弧 ，单位0.5度，范围0~720度，竖直方向为0，顺时针方向增加
void ui_DrawArc(u16 x, u16 y, u16 r, u16 as, u16 ae);

//方框square
void ui_DrawSquare(u16 xs, u16 ys, u16 xe, u16 ye, u8 mod);

//方块填充前景色
void ui_FFill(u16 xs, u16 ys, u16 xe, u16 ye);

//方块填充背景色
void ui_BFill(u16 xs, u16 ys, u16 xe, u16 ye);

//多个同心方框square
void ui_DrawSquareConcentric(u16 xs, u16 ys, u16 xe, u16 ye, u8 mod, u16 dx, u16 dy, u8 num);

//区域填充 凸多边形
// x,y 种子点
// color 填充色
void ui_RegionFill(u16 x, u16 y, u16 color);

//指定区域横向滚屏
// mod: 0右环移，1左环移，2右移，3左移
void ui_RectHScroll(u16 xs, u16 ys, u16 xe, u16 ye, u8 offset, u8 mod);

//不知为何 使用一次ICO后影响文字显示，必须再次使用一次无效ICO号
// ico显示 ICO放在lib_id=0x28=40，文件名为40.ICO 占据4个id宽度 。一个ICO文件内部有多张图片
// mod:0 不显示ICO背景（透明） ，其他 不过滤
// n :ICO内部索引，文件名按照10进制
void _ui_ShowIcon(u16 x, u16 y, u16 n, u8 mod);

void ui_ShowIcon(u16 x, u16 y, u16 n, u8 mod);

//显示编号BMP:
//此编号 与SD卡内图片编号一样，是按照10进制。比如10.bmp,是10号图片
void ui_ShowBmp(u8 n);

//保存图片到HMI ，n对应ui_ShowBmp(u8 n)
void ui_StoreBmpTo(u8 n);

//显示图片，并获取CRC-16
void ui_ShowBmpGetCRC(u8 n);

//剪切编号图片某个区域，到屏幕位置线，（x,y）
//区别在于多次剪切
void ui_CutAndShowBmp(u8 n, u16 xs, u16 ys, u16 xe, u16 ye, u16 x, u16 y, u8 mod);

//剪切并旋转显示到屏幕位置
// mod:0 透明剪切，1不透明
void ui_CutAndRotateShowBmp(u8 n, u16 xs, u16 ys, u16 xe, u16 ye, u16 xc0, u16 yc0, u16 al, u16 xc1, u16 yc1, u8 mod);

//设置文本区域限制
void ui_SetTextboxLimit(u16 xs, u16 ys, u16 xe, u16 ye);

//取消文本显示区域
void ui_ResetTextboxLimit(void);

//显示单个ASCII
void ui_StandardShowChar(u16 x, u16 y, u8 size, u8 hex);

//显示字符串GBK
void ui_StandardShowString(u16 x, u16 y, u8 size, char *s);

//显示数组方式的字符串
void ui_StandardShowArr(u16 x, u16 y, u8 size, char *arr, uint8_t len);

//显示数字
// len 占据个数
void ui_StandardShowNum(u16 x, u16 y, u8 size, int num);

//带空格占长度
void ui_StandardShowLenNum(u16 x, u16 y, u8 size, u8 len, int num);

//调节背光
void ui_SetBkl(u8 pwm);

//模仿printf方式把字符打印到串口屏，换行符\r\n要与ui_SetTextboxLimit()配合使用
void ui_print(u16 x, u16 y, u8 size, char *str, ...);

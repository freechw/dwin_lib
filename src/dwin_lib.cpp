// https://blog.csdn.net/wangzibigan/article/details/86003616

#include <Arduino.h>
#include <cstdlib>
#include "dwin_lib.h"

#define send_data(data) Serial.write(data)

/* Color coding (16-bit):
     15..11 = R4..0 (Red)
     10..5  = G5..0 (Green)
      4..0  = B4..0 (Blue)
*/

/* GLCD RGB color definitions                            */
#define GLCD_COLOR_BLACK 0x0000      /*   0,   0,   0 */
#define GLCD_COLOR_NAVY 0x000F       /*   0,   0, 128 */
#define GLCD_COLOR_DARK_GREEN 0x03E0 /*   0, 128,   0 */
#define GLCD_COLOR_DARK_CYAN 0x03EF  /*   0, 128, 128 */
#define GLCD_COLOR_MAROON 0x7800     /* 128,   0,   0 */
#define GLCD_COLOR_PURPLE 0x780F     /* 128,   0, 128 */
#define GLCD_COLOR_OLIVE 0x7BE0      /* 128, 128,   0 */
#define GLCD_COLOR_LIGHT_GREY 0xC618 /* 192, 192, 192 */
#define GLCD_COLOR_DARK_GREY 0x7BEF  /* 128, 128, 128 */
#define GLCD_COLOR_BLUE 0x001F       /*   0,   0, 255 */
#define GLCD_COLOR_GREEN 0x07E0      /*   0, 255,   0 */
#define GLCD_COLOR_CYAN 0x07FF       /*   0, 255, 255 */
#define GLCD_COLOR_RED 0xF800        /* 255,   0,   0 */
#define GLCD_COLOR_MAGENTA 0xF81F    /* 255,   0, 255 */
#define GLCD_COLOR_YELLOW 0xFFE0     /* 255, 255, 0   */
#define GLCD_COLOR_WHITE 0xFFFF      /* 255, 255, 255 */

u8 g_fcbc_useless = 0;                                       //前景色背景色变量是否为无效
u16 g_fcolor = GLCD_COLOR_WHITE, g_bcolor = GLCD_COLOR_BLUE; //默认

//设置前景色与背景色
void ui_SetFCBC(u16 fc, u16 bc)
{
    g_fcbc_useless = 0;
    g_fcolor = fc;
    g_bcolor = bc;
    send_data(0xAA);
    send_data(0x40);
    // FC
    send_data(fc >> 8);
    send_data(fc);
    // BC
    send_data(bc >> 8);
    send_data(bc);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//取某点颜色到前景色
void ui_GetPointToFC(u16 x, u16 y)
{
    send_data(0xAA);
    send_data(0x43);
    // x
    send_data(x >> 8);
    send_data(x);
    // y
    send_data(y >> 8);
    send_data(y);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
    g_fcbc_useless = 1;
}

//取某点颜色到背景色
void ui_GetPointToBC(u16 x, u16 y)
{
    send_data(0xAA);
    send_data(0x42);
    // x
    send_data(x >> 8);
    send_data(x);
    // y
    send_data(y >> 8);
    send_data(y);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
    g_fcbc_useless = 1;
}

//设置字符间距和行间距
void ui_SetInterval(u8 dx, u8 dy)
{
    send_data(0xAA);
    send_data(0x41);
    // dx
    send_data(dx);
    // dy
    send_data(dy);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//设置显示光标
// T5UIC2不支持本命令！！！
void ui_SetShowCursor(u16 x, u16 y, u8 dx, u8 dy, u8 en)
{
    send_data(0xAA);
    send_data(0x44);
    send_data(en);
    // x
    send_data(x >> 8);
    send_data(x);
    // y
    send_data(y >> 8);
    send_data(y);
    // dx dy
    send_data(dx);
    send_data(dy);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//使用背景色清屏
void ui_Clear(void)
{
    send_data(0xAA);
    send_data(0x52); //
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//画点
// mod:0 恢复点背景色 ，1 前景色设置点
void ui_DrawPointMulti(u8 mod, pointType po[], u8 num)
{
    u16 i;
    send_data(0xAA);
    send_data(0x50 + mod); //画点命令：0x51使用前景色,0x52使用背景色
    for (i = 0; i < num; i++)
    {
        // x
        send_data((po[i].x) >> 8);
        send_data(po[i].x);
        // y
        send_data((po[i].y) >> 8);
        send_data(po[i].y);
    }
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//连线
// mod:0  背景色 ，1  前景色
void ui_DrawLineMulti(u8 mod, pointType po[], u8 num)
{
    u16 i;
    send_data(0xAA);
    send_data(0x5d - mod * (0x5d - 0x56)); //画点命令：0x51使用前景色,0x52使用背景色
    for (i = 0; i < num; i++)
    {
        // x
        send_data((po[i].x) >> 8);
        send_data(po[i].x);
        // y
        send_data((po[i].y) >> 8);
        send_data(po[i].y);
    }
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//画频谱
void ui_DrawSpectrogram(u16 x, u16 y, u16 maxH, u8 arr[], u8 num)
{
    u16 i;
    send_data(0xAA);
    send_data(0x75); // 画频谱命令
    // x
    send_data(x >> 8);
    send_data(x);
    // y
    send_data(y >> 8);
    send_data(y);
    //最大高度maxH
    if (maxH > 0xff)
    {
        send_data(0);
        send_data(maxH >> 8);
        send_data(maxH);

        for (i = 0; i < num; i++)
        {
            //高度
            send_data(arr[i] >> 8);
            send_data(arr[i]);
        }
    }
    else
    {
        send_data(maxH);
        //高度
        for (i = 0; i < num; i++)
        {
            //高度
            send_data(arr[i]);
        }
    }
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//固定X间隔的折线图
void ui_Polyline(u16 x, u16 dx, u8 arr[], u8 num)
{
    send_data(0xAA);
    send_data(0x76); // 命令
    // TODO

    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//画圆
// mod:0圆线反色，1正常画圆线，2圆区域反色，3正常实心圆
// T5UIC2只支持mod=1,3 !!!
void ui_DrawCircle(u16 x, u16 y, u8 r, u8 mod)
{
    send_data(0xAA);
    send_data(0x57); // 画圆命令

    send_data(mod); // 模式
    // x
    send_data(x >> 8);
    send_data(x);
    // y
    send_data(y >> 8);
    send_data(y);
    // r
    send_data(r);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//多个同心圆
void ui_DrawCircleConcentric(u16 x, u16 y, u8 r, u8 mod, u8 dr, u8 num)
{
    u8 i;
    send_data(0xAA);
    send_data(0x57); // 画圆命令
    for (i = 0; i < num; i++)
    {
        r += dr;
        send_data(mod); // 模式
        // x
        send_data(x >> 8);
        send_data(x);
        // y
        send_data(y >> 8);
        send_data(y);
        // r
        send_data(r);
    }
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//画圆弧 ，单位0.5度，范围0~720度，竖直方向为0，顺时针方向增加
void ui_DrawArc(u16 x, u16 y, u16 r, u16 as, u16 ae)
{
    send_data(0xAA);
    send_data(0x57); // 画圆命令
    send_data(0x04); // 模式为画圆弧
    // x
    send_data(x >> 8);
    send_data(x);
    // y
    send_data(y >> 8);
    send_data(y);
    // r
    send_data(r >> 8);
    send_data(r);
    // as
    send_data(as >> 8);
    send_data(as);
    // ae
    send_data(ae >> 8);
    send_data(ae);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//方框square
void ui_DrawSquare(u16 xs, u16 ys, u16 xe, u16 ye, u8 mod)
{
    switch (mod)
    {
    case 0:
        mod = 0x59;
        break; //前景色画框
    case 1:
        mod = 0x69;
        break; //背景色画框
    case 2:
        mod = 0x5B;
        break; //前景色填充
    case 3:
        mod = 0x5A;
        break; //背景色填充
    case 4:
        mod = 0x5C;
        break; //区域取反
    default:
        return;
    }
    send_data(0xAA);
    send_data(mod);
    // xs
    send_data(xs >> 8);
    send_data(xs);
    // ys
    send_data(ys >> 8);
    send_data(ys);
    // xe
    send_data(xe >> 8);
    send_data(xe);
    // ye
    send_data(ye >> 8);
    send_data(ye);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//方块填充前景色
void ui_FFill(u16 xs, u16 ys, u16 xe, u16 ye)
{
    ui_DrawSquare(xs, ys, xe, ye, 2);
}

//方块填充背景色
void ui_BFill(u16 xs, u16 ys, u16 xe, u16 ye)
{
    ui_DrawSquare(xs, ys, xe, ye, 3);
}

//多个同心方框square
void ui_DrawSquareConcentric(u16 xs, u16 ys, u16 xe, u16 ye, u8 mod, u16 dx, u16 dy, u8 num)
{
    u8 i;

    switch (mod)
    {
    case 0:
        mod = 0x59;
        break; //前景色画框
    case 1:
        mod = 0x69;
        break; //背景色画框
    case 2:
        mod = 0x5B;
        break; //前景色填充
    case 3:
        mod = 0x5A;
        break; //背景色填充
    case 4:
        mod = 0x5C;
        break; //区域取反
    }
    send_data(0xAA);
    for (i = 0; i < num; i++)
    {
        xs -= dx;
        ys -= dy;
        xe += dx;
        ye += dy;
        send_data(mod);
        // xs
        send_data(xs >> 8);
        send_data(xs);
        // ys
        send_data(ys >> 8);
        send_data(ys);
        // xe
        send_data(xe >> 8);
        send_data(xe);
        // ye
        send_data(ye >> 8);
        send_data(ye);
    }
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//区域填充 凸多边形
// x,y 种子点
// color 填充色
void ui_RegionFill(u16 x, u16 y, u16 color)
{
    send_data(0xAA);
    send_data(0x64); //命令
    // x
    send_data(x >> 8);
    send_data(x);
    // y
    send_data(y >> 8);
    send_data(y);
    // c
    send_data(color >> 8);
    send_data(color);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//指定区域横向滚屏
// mod: 0右环移，1左环移，2右移，3左移
void ui_RectHScroll(u16 xs, u16 ys, u16 xe, u16 ye, u8 offset, u8 mod)
{
    send_data(0xAA);
    send_data(60 + mod); //命令
    // xs
    send_data(xs >> 8);
    send_data(xs);
    // ys
    send_data(ys >> 8);
    send_data(ys);
    // xe
    send_data(xe >> 8);
    send_data(xe);
    // ye
    send_data(ye >> 8);
    send_data(ye);
    // offset
    send_data(offset);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//不知为何 使用一次ICO后影响文字显示，必须再次使用一次无效ICO号
// ico显示 ICO放在lib_id=0x28=40，文件名为40.ICO 占据4个id宽度 。一个ICO文件内部有多张图片
// mod:0 不显示ICO背景（透明） ，其他 不过滤
// n :ICO内部索引，文件名按照10进制
void _ui_ShowIcon(u16 x, u16 y, u16 n, u8 mod)
{
    send_data(0xAA);
    send_data(0x97);
    // x
    send_data(x >> 8);
    send_data(x);
    // y
    send_data(y >> 8);
    send_data(y);
    // lib_ID
    send_data(0x28);
    //模式
    send_data(mod);
    // n
    send_data(n >> 8);
    send_data(n);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

void ui_ShowIcon(u16 x, u16 y, u16 n, u8 mod)
{
    _ui_ShowIcon(x, y, n, mod);
    _ui_ShowIcon(x, y, 0x1010, mod);
}

//显示编号BMP:
//此编号 与SD卡内图片编号一样，是按照10进制。比如10.bmp,是10号图片
void ui_ShowBmp(u8 n)
{
    send_data(0xAA);
    send_data(0x70); //命令
    //图片编号
    send_data(n);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//保存图片到HMI ，n对应ui_ShowBmp(u8 n)
void ui_StoreBmpTo(u8 n)
{
    if (n > 4 && n < 128)
    {
        send_data(0xAA);
        send_data(0xE2); //命令
        //图片编号
        send_data(n);
        // END
        send_data(0xCC);
        send_data(0x33);
        send_data(0xC3);
        send_data(0x3C);
    }
}

//显示图片，并获取CRC-16
void ui_ShowBmpGetCRC(u8 n)
{
    send_data(0xAA);
    send_data(0x7B); //命令
    //图片编号
    send_data(n);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//剪切编号图片某个区域，到屏幕位置线，（x,y）
//区别在于多次剪切
void ui_CutAndShowBmp(u8 n, u16 xs, u16 ys, u16 xe, u16 ye, u16 x, u16 y, u8 mod)
{
    switch (mod)
    {
    case 0:
        mod = 0x71;
        break; // 正常剪切
    case 1:
        mod = 0x9C;
        break; // 透明效果，恢复最初背景（要求被切区域 背景纯色）
    case 2:
        mod = 0x9D;
        break; // 恢复之前背景（要求被切区域 背景纯色）
    default:
        return;
    }
    send_data(0xAA);
    send_data(mod); //命令
    //图片编号
    send_data(n);
    // xs
    send_data(xs >> 8);
    send_data(xs);
    // ys
    send_data(ys >> 8);
    send_data(ys);
    // xe
    send_data(xe >> 8);
    send_data(xe);
    // ye
    send_data(ye >> 8);
    send_data(ye);
    // x
    send_data(x >> 8);
    send_data(x);
    // y
    send_data(y >> 8);
    send_data(y);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//剪切并旋转显示到屏幕位置
// mod:0 透明剪切，1不透明
void ui_CutAndRotateShowBmp(u8 n, u16 xs, u16 ys, u16 xe, u16 ye, u16 xc0, u16 yc0, u16 al, u16 xc1, u16 yc1, u8 mod)
{
    send_data(0xAA);
    send_data(0x9E);
    send_data(mod); //模式
    //图片编号
    send_data(0);
    send_data(n);
    // xs
    send_data(xs >> 8);
    send_data(xs);
    // ys
    send_data(ys >> 8);
    send_data(ys);
    // xe
    send_data(xe >> 8);
    send_data(xe);
    // ye
    send_data(ye >> 8);
    send_data(ye);
    // xc0
    send_data(xc0 >> 8);
    send_data(xc0);
    // yc0
    send_data(yc0 >> 8);
    send_data(yc0);
    // al
    send_data(al >> 8);
    send_data(al);

    // xc1
    send_data(xc1 >> 8);
    send_data(xc1);
    // yc1
    send_data(yc1 >> 8);
    send_data(yc1);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//设置文本区域限制
void ui_SetTextboxLimit(u16 xs, u16 ys, u16 xe, u16 ye)
{
    send_data(0xAA);
    send_data(0x45);
    // xs
    send_data(xs >> 8);
    send_data(xs);
    // ys
    send_data(ys >> 8);
    send_data(ys);
    // xe
    send_data(xe >> 8);
    send_data(xe);
    // ye
    send_data(ye >> 8);
    send_data(ye);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//取消文本显示区域
void ui_ResetTextboxLimit(void)
{
    send_data(0xAA);
    send_data(0x45);
    send_data(0x00);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//显示单个ASCII
void ui_StandardShowChar(u16 x, u16 y, u8 size, u8 hex)
{
    char cmd;
    switch (size)
    {
    case 12:
        cmd = 0x6E;
        break;
    case 16:
        cmd = 0x54;
        break;
    case 24:
        cmd = 0x6F;
        break;
    case 32:
        cmd = 0x55;
        break;
    default:
        return;
    }
    send_data(0xAA);
    send_data(cmd);
    // X,Y
    send_data(x >> 8);
    send_data(x);
    send_data(y >> 8);
    send_data(y);
    // STRING
    send_data(hex);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//显示字符串GBK
void ui_StandardShowString(u16 x, u16 y, u8 size, char *s)
{
    char cmd;
    switch (size)
    {
    case 12:
        cmd = 0x6E;
        break;
    case 16:
        cmd = 0x54;
        break;
    case 24:
        cmd = 0x6F;
        break;
    case 32:
        cmd = 0x55;
        break;
    default:
        return;
    }
    send_data(0xAA);
    send_data(cmd);
    // X,Y
    send_data(x >> 8);
    send_data(x);
    send_data(y >> 8);
    send_data(y);
    // STRING
    while (*s != '\0')
    {
        send_data(*s);
        s++;
    }
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}
//显示数组方式的字符串
void ui_StandardShowArr(u16 x, u16 y, u8 size, char *arr, uint8_t len)
{
    char cmd;
    uint8_t i = 0;
    switch (size)
    {
    case 12:
        cmd = 0x6E;
        break;
    case 16:
        cmd = 0x54;
        break;
    case 24:
        cmd = 0x6F;
        break;
    case 32:
        cmd = 0x55;
        break;
    default:
        return;
    }
    send_data(0xAA);
    send_data(cmd);
    // X,Y
    send_data(x >> 8);
    send_data(x);
    send_data(y >> 8);
    send_data(y);
    // ARR

    while (i < len)
    {
        send_data(arr[i]);
        i++;
    }
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//显示数字
// len 占据个数
void ui_StandardShowNum(u16 x, u16 y, u8 size, int num)
{
    // char *p = mymalloc(12);
    char *p;
    // if (p != NULL)
    {
        sprintf(p, "%d", num);
        ui_StandardShowString(x, y, size, p);
        free(p);
    }
}
//带空格占长度
void ui_StandardShowLenNum(u16 x, u16 y, u8 size, u8 len, int num)
{
    char *p;
    if (len <= 12)
    {
        // p = mymalloc(12);
    }
    else
    {
        // p = mymalloc(len + 1);
    }
    // if (p != NULL)
    {
        sprintf(p, "%*d", len, num);
        ui_StandardShowString(x, y, size, p);
        // myfree(p);
    }
}

//调节背光
void ui_SetBkl(u8 pwm)
{
    send_data(0xAA);
    send_data(0x5F); //命令
    //亮度 0-0X3F
    send_data(pwm);
    // END
    send_data(0xCC);
    send_data(0x33);
    send_data(0xC3);
    send_data(0x3C);
}

//模仿printf方式把字符打印到串口屏，换行符\r\n要与ui_SetTextboxLimit()配合使用
void ui_print(u16 x, u16 y, u8 size, char *str, ...)
{
    s16 len;
    char *p;
    va_list pArgs;

    va_start(pArgs, str);
    len = vsnprintf(NULL, 0, str, pArgs) + 1; //计算内容长度 失败返回负数
    if (len > 0)
    {
        // p = mymalloc(len); //申请内存
        // if (p != NULL)
        {
            vsnprintf(p, len, str, pArgs); //格式化成字符串
            ui_StandardShowString(x, y, size, p);
            // myfree(p);
        }
        // else
        //     ui_StandardShowString(x, y, size, "lcd print malloc err!");
    }
    va_end(pArgs);
}

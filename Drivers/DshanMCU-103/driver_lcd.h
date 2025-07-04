// SPDX-License-Identifier: GPL-3.0-only
/*
 * Copyright (c) 2008-2023 100askTeam : Dongshan WEI <weidongshan@qq.com> 
 * Discourse:  https://forums.100ask.net
 */

 
/*  Copyright (C) 2008-2023 深圳百问网科技有限公司
 *  All rights reserved
 *
 *
 * 免责声明: 百问网编写的文档，仅供学员学习使用，可以转发或引用(请保留作者信息)，禁止用于商业用途！
 * 免责声明: 百问网编写的程序，可以用于商业用途，但百问网不承担任何后果！
 * 
 * 摘要：
 *			本程序遵循GPL V3协议，使用请遵循协议许可
 *			本程序所用的开发板：	DShanMCU-F103
 *			百问网嵌入式学习平台：https://www.100ask.net
 *			百问网技术交流社区：	https://forums.100ask.net
 *			百问网官方B站：				https://space.bilibili.com/275908810
 *			百问网官方淘宝：			https://100ask.taobao.com
 *			联系我们(E-mail)：	  weidongshan@qq.com
 *
 *			版权所有，盗版必究。
 *  
 * 修改历史     版本号           作者        修改内容
 *-----------------------------------------------------
 * 2023.08.04      v01         百问科技      创建文件
 *-----------------------------------------------------
*/

#ifndef __DRIVER_LCD_H
#define __DRIVER_LCD_H

#include <stdint.h>

/*
 *  函数名：LCD_Init
 *  功能描述：初始化LCD
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 */
void LCD_Init(void);


/*
 *  函数名：LCD_Clear
 *  功能描述：清屏函数
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
*/
void LCD_Clear(void);

/*
 *  函数名：LCD_PutChar
 *  功能描述：显示一个字符
 *  输入参数：x --> x坐标
 *            y --> y坐标
 *            c -->   显示的字符
 *  输出参数：无
 *  返回值：无
*/
void LCD_PutChar(uint8_t x, uint8_t y, char c);


/*
 *  函数名：LCD_PrintString
 *  功能描述：显示一个字符串
 *  输入参数：x --> x坐标
 *            y --> y坐标
 *            str -->   显示的字符串
 *  输出参数：无
 *  返回值：打印了多少个字符
*/
int LCD_PrintString(uint8_t x, uint8_t y, const char *str);

/*
 *  函数名：OLED_ClearLine
 *  功能描述：清除一行
 *  输入参数：x - 从这里开始
 *            y - 清除这行
 *  输出参数：无
 *  返回值：无
 */
void LCD_ClearLine(uint8_t x, uint8_t y);

/*
 *  LCD_PrintHex
 *  功能描述：以16进制显示数值
 *  输入参数：x - x坐标
 *            y - y坐标
 *            val -   显示的数据
 *            pre -   非零时显示"0x"前缀
 *  输出参数：无
 *  返回值：打印了多少个字符
 */
int LCD_PrintHex(uint8_t x, uint8_t y, uint32_t val, uint8_t pre);

/*
 *  LCD_PrintSignedVal
 *  功能描述：以10进制显示一个数值
 *  输入参数：x --> x坐标(0~15)
 *            y --> y坐标(0~7)
 *            val -->   显示的数据
 *  输出参数：无
 *  返回值：打印了多少个字符
 */
int LCD_PrintSignedVal(uint8_t x, uint8_t y, int32_t val);


/**********************************************************************
 * 函数名称： LCD_Test
 * 功能描述： LCD测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2023/08/03        V1.0     韦东山       创建
 ***********************************************************************/
void LCD_Test(void);

#endif /* __DRIVER_LCD_H */


/*

 * 这个头文件是oled库的 [硬件层] 实现文件，移植的时候需要更改这个文件的内容

*/
#include "OLED_driver.h"


extern I2C_HandleTypeDef hi2c1;



uint8_t OLED_DisplayBuf[64/8][128];

bool OLED_ColorMode = true;

/*------------------------------修改为直接使用hal库的硬件IIC------------------------------*/
/**
 * @brief  OLED写命令函数
 * @note   使用HAL库I2C函数实现
 * @param  Command: 要写入的命令
 * @retval None
 */
void OLED_WriteCommand(uint8_t Command)
{
    uint8_t data[2] = {0x00, Command}; // 控制字节0x00，表示写命令
    HAL_I2C_Master_Transmit(&hi2c1, 0x78, data, 2, 100);
}

/**
 * @brief 设置显示模式
 * @param colormode true: 黑色模式，false: 白色模式
 * @return 无
 */
void OLED_SetColorMode(bool colormode){
	OLED_ColorMode = colormode;
}
/**
  * 函    数：OLED写数据
  * 参    数：Data 要写入数据的起始地址
  * 参    数：Count 要写入数据的数量
  * 返 回 值：无
  */
void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
    uint8_t buffer[129]; // 最大128+控制字节
    buffer[0] = 0x40;    // 控制字节0x40，表示写数据
    
    for (uint8_t i = 0; i < Count; i++)
    {
        buffer[i+1] = OLED_ColorMode ? Data[i] : ~Data[i];
    }
    
    HAL_I2C_Master_Transmit(&hi2c1, 0x78, buffer, Count+1, 100);
}

/*------------------------------修改为直接使用hal库的硬件IIC------------------------------*/
/*********************通信协议*/

/**
  * 函    数：OLED设置显示光标位置
  * 参    数：Page 指定光标所在的页，范围：0~7
  * 参    数：X 指定光标所在的X轴坐标，范围：0~127
  * 返 回 值：无
  * 说    明：OLED默认的Y轴，只能8个Bit为一组写入，即1页等于8个Y轴坐标
  */
void OLED_SetCursor(uint8_t Page, uint8_t X)
{
	/*如果使用此程序驱动1.3寸的OLED显示屏，则需要解除此注释*/
	/*因为1.3寸的OLED驱动芯片（SH1106）有132列*/
	/*屏幕的起始列接在了第2列，而不是第0列*/
	/*所以需要将X加2，才能正常显示*/
//	X += 2;
	
	/*通过指令设置页地址和列地址*/
	OLED_WriteCommand(0xB0 | Page);					//设置页位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}
void OLED_Clear(void);
/**
  * 函    数：OLED初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：使用前，需要调用此初始化函数
  */
void OLED_Init(void)
{
	//先调用底层的端口初始化，在cubemx中已经自动调用了
	
	uint8_t cmd;
    
    /* 发送初始化命令序列 */
    cmd = 0xAE; // 关闭显示
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0x20; // 设置内存寻址模式
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    cmd = 0x00; // 水平寻址模式
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0xC8; // 翻转COM扫描方向
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0x40; // 设置显示起始行
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0xA1; // 翻转段重映射
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0xA8; // 设置复用比
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    cmd = 0x3F; // 64MUX
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0xD3; // 设置显示偏移
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    cmd = 0x00; // 偏移0
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0xD5; // 设置显示时钟分频
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    cmd = 0x80; // 推荐值
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0xD9; // 设置预充电周期
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    cmd = 0xF1; // 推荐值
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0xDB; // 设置VCOMH电平
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    cmd = 0x30; // 0.83xVcc
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0x8D; // 电荷泵设置
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    cmd = 0x14; // 启用内部VCC
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0xA4; // 显示RAM内容
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0xA6; // 正常显示（非反显）
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    cmd = 0xAF; // 开启显示
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    
    /* 清屏 */
    OLED_Clear();
    OLED_Update();
}
/**
  * 函    数：将OLED显存数组更新到OLED屏幕
  * 参    数：无
  * 返 回 值：无
  * 说    明：所有的显示函数，都只是对OLED显存数组进行读写
  *           随后调用OLED_Update函数或OLED_UpdateArea函数
  *           才会将显存数组的数据发送到OLED硬件，进行显示
  *           故调用显示函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_Update(void)
{
	uint8_t j;
	/*遍历每一页*/
	for (j = 0; j < 8; j ++)
	{
		/*设置光标位置为每一页的第一列*/
		OLED_SetCursor(j, 0);
		/*连续写入128个数据，将显存数组的数据写入到OLED硬件*/
		OLED_WriteData(OLED_DisplayBuf[j], 128);
	}
	
	
}

/**
  * 函    数：OLED设置亮度
  * 参    数：Brightness ，0-255，不同显示芯片效果可能不相同。
  * 返 回 值：无
  * 说    明：不要设置过大或者过小。
  */
void OLED_Brightness(int16_t Brightness){
	if(Brightness>255){
		Brightness=255;
	}
	if(Brightness<0){
		Brightness=0;
	}
	OLED_WriteCommand(0x81);
	OLED_WriteCommand(Brightness);
}
/**
  * 函    数：将OLED显存数组部分更新到OLED屏幕
  * 参    数：X 指定区域左上角的横坐标，范围：0~OLED_WIDTH-1
  * 参    数：Y 指定区域左上角的纵坐标，范围：0~OLED_HEIGHT-1
  * 参    数：Width 指定区域的宽度，范围：0~OLED_WIDTH
  * 参    数：Height 指定区域的高度，范围：0~OLED_HEIGHT
  * 返 回 值：无
  * 说    明：此函数会至少更新参数指定的区域
  *           如果更新区域Y轴只包含部分页，则同一页的剩余部分会跟随一起更新
  * 说    明：所有的显示函数，都只是对OLED显存数组进行读写
  *           随后调用OLED_Update函数或OLED_UpdateArea函数
  *           才会将显存数组的数据发送到OLED硬件，进行显示
  *           故调用显示函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t j;
	
	/*参数检查，保证指定区域不会超出屏幕范围*/
	if (X > 128-1) {return;}
	if (Y > 64-1) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}
	
	/*遍历指定区域涉及的相关页*/
	/*(Y + Height - 1) / 8 + 1的目的是(Y + Height) / 8并向上取整*/
	for (j = Y / 8; j < (Y + Height - 1) / 8 + 1; j ++)
	{
		/*设置光标位置为相关页的指定列*/
		OLED_SetCursor(j, X);
		/*连续写入Width个数据，将显存数组的数据写入到OLED硬件*/
		OLED_WriteData(&OLED_DisplayBuf[j][X], Width);
	}
	
}








HW04
===
This is the hw04 sample. Please follow the steps below.

# Build the Sample Program

1. Fork this repo to your own github account.

2. Clone the repo that you just forked.

3. Under the hw04 dir, use:

	* `make` to build.

	* `make flash` to flash the binary file into your STM32F4 device.

	* `make clean` to clean the ouput files.

# Build Your Own Program

1. Edit or add files if you need to.

2. Make and run like the steps above.

3. Please avoid using hardware dependent C standard library functions like `printf`, `malloc`, etc.

# HW04 Requirements

1. Please practice to reference the user manuals mentioned in [Lecture 04], and try to use the user button (the blue one on the STM32F4DISCOVERY board).

2. After reset, the device starts to wait until the user button has been pressed, and then starts to blink the blue led forever.

3. Try to define a macro function `READ_BIT(addr, bit)` in reg.h for reading the value of the user button.

4. Push your repo to your github. (Use .gitignore to exclude the output files like object files, executable files, or binary files)

5. The TAs will clone your repo, build from your source code, and flash to see the result.

[Lecture 04]: http://www.nc.es.ncku.edu.tw/course/embedded/04/

--------------------

- [x] **If you volunteer to give the presentation (demo) next week, check this.**

--------------------

## User Button

### Button 電路結構
Discovery kit with STM32F407VG MCU ( Page 31 )

![](https://i.imgur.com/ZJZ922u.png)

由 section 6.4 以及上方電路圖節可知，User Button 是連接在 I/O PA0 上

![](https://i.imgur.com/alPZxBT.png)

## GPIO

### GPIO 

由 8.1 節可知每個 GPIO 所擁有的 registers

![](https://i.imgur.com/7aqesSi.png)

* configuration reg
    * MODER
    * OTYPER
    * OSPEEDR
    * PUPDR
* data reg
    * IDR
    * ODR
* set/reset reg
    * BSRR
* locking reg
    * LCKR
* alternate reg
    * AFRH
    * AFRL

### GPIO 參數設置
可參考 8.3 節 Table 35
![](https://i.imgur.com/9zsRv1Y.png)
![](https://i.imgur.com/1XgHEyN.png)

#### Input type

![](https://i.imgur.com/8dINa99.png)

Input type 選擇 `Floating`
故 reg 設置如下
* MODER `00`
* PUPDR `00`

![](https://i.imgur.com/3msJnMh.png)


```clike=
// MODER = 00 
CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_1_BIT(GPIO_PORTA));
CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_0_BIT(GPIO_PORTA));

// PUPDR = 00 
CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(GPIO_PORTA));
CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(GPIO_PORTA));
```

#### Read Data

由 section 6.4 可知，若要讀取 Button 狀態，需讀取 PA0
故先查詢 PA0 之記憶體位置
![](https://i.imgur.com/6UPNpcs.png)

可得知其 offset 為 0x10
寫一個 Marco 計算 Reg 位置
```clike=
#define GPIOx_IDR_OFFSET 0x10
#define IDR_1_BIT(y) ((y)*2+1)
#define IDR_0_BIT(y) ((y)*2)
```
寫一個 Read_Bit Marco 取得特定位數 bit 資料
```clike=
#define READ_BIT(addr, bit) ( ( REG(addr) >> bit ) &  UINT32_1 )
```

#### Program Flow
1.GPIO initialization
```clike=
void GPIO_init() {
	// before using gpio need set rcc
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTA));

	// MODER = 00 => floating
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_1_BIT(GPIO_PORTA));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_0_BIT(GPIO_PORTA));

	// PUPDR = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(GPIO_PORTA));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(GPIO_PORTA));
}
```
2.LED init
```clike=
void led_init(unsigned int led)
{
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTD));

	//MODER led pin = 01 => General purpose output mode
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_1_BIT(led));
	SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_0_BIT(led));

	//OT led pin = 0 => Output push-pull
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OTYPER_OFFSET, OTy_BIT(led));

	//OSPEEDR led pin = 00 => Low speed
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(led));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(led));

	//PUPDR led pin = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(led));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(led));
}
```
3.Wait for button trigger
```clike=
while (! ( READ_BIT( GPIO_BASE(GPIO_PORTA) + GPIOx_IDR_OFFSET , IDRy_0_BIT(GPIO_PORTA) ) ) ) ;
```
4.Blink LED






參考資料
[GPIO](http://wiki.csie.ncku.edu.tw/embedded/GPIO)

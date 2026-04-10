## Що додати в `Core/Src/main.c` (USER CODE)

CubeMX згенерує `main.c`. Після генерації додай (або встав) ці рядки в відповідні секції:

### 1) Підключення заголовка

У верхній частині `main.c`, в секції:
`/* USER CODE BEGIN Includes */`

додай:

```c
#include "app_main.h"
```

### 2) Виклик `app_init()`

У `main()` після `MX_..._Init()` викликів, в секції:
`/* USER CODE BEGIN 2 */`

додай:

```c
app_init();
```

### 3) Виклик `app_loop()`

У нескінченному циклі `while (1)` в секції:
`/* USER CODE BEGIN WHILE */`

додай:

```c
app_loop();
```


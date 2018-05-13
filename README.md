# API

* ## 1. Клиент просит все поле (инициализация)

	* ### Запрос:

	```javascript
	{
		"command": "get_field"
	}
	```

	* ### Ответ:

	```javascript	
	{
		"status": "OK",
		"field": [
			{
				"color": 1,
				"unlock_time": "12-01-2017 14:15:11",
			},
			......
			......
			{
				"color": 7,
				"unlock_time": "13-01-2017 14:17:15"
			}
		]
	}
	```

* ## 2. Регулярный запрос (5 сек) пробегаем по незаблоченым пикселям и запрашиваем информацию по их изменению

	* ### Запрос:

	```javascript
	{
		"command": "update_field",
		"pixels_for_update": [20, 21, 46, 80, 3, 7, 15]
	}
	```
	
	* ### Ответ:

	```javascript
	{
		"status": "OK", 
		"updated_pixels": [
			{
				"id": 20,
				"color": 1,
				"unlock_time": "12-01-2017 14:15:11",
			},
			{
				"id": 21,
				"color": 1,
				"unlock_time": "12-01-2017 14:15:11",
			},
			{
				"id": 46,
				"color": 1,
				"unlock_time": "12-01-2017 14:15:11",
			},
			{
				"id": 15,
				"color": 7,
				"unlock_time": "13-01-2017 14:17:15"
			}
		]
	}
	```

3. Клиент закрасил пиксель

	* ### Запрос:

	```javascript
	{
		"command": "paint_pixel",
		"pixel_for_paint": {
			"id": 42,
			"color": 7
		}
	}
	```

	* ### Ответ:

	** A)

	```javascript
	{
		"status": "OK",
		"unlock_time": "14-04-2017 15:12:36"
	}
	```

	** Б)

	```javascript
	{
		"status": "FAIL",
		"failed_pixel": {
			"color": 20,
			"unlock_time": "14-04-2017 15:12:36"
		}
	}
	```

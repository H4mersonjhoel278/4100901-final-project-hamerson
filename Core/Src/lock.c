#include "lock.h"
#include "ring_buffer.h"
#include "keypad.h"
#include "main.h"
#include "gui.h"

#include <stdio.h>
#include <string.h>
#include "gui.h"

#include "ssd1306.h"
#include "ssd1306_fonts.h"
//agregamos para las imagenes
extern const uint8_t carita[];
extern const uint8_t mano[];
extern const uint8_t bien[];
uint8_t logged = 0;

// Función para encender el LED
void Encender_LED(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

// Función para apagar el LED
void Apagar_LED(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}
#define MAX_PASSWORD 10
//para que la clave sea mas secreta y mas dificil de ingresar o hackear
//se puso una contraseña de 6 numeros
uint8_t password[MAX_PASSWORD] = "278115";


uint8_t keypad_buffer[MAX_PASSWORD];
ring_buffer_t keypad_rb;


extern volatile uint16_t keypad_event;


static uint8_t lock_get_passkey(void)
{
	// Espera hasta que se presione una tecla en el teclado
	while (ring_buffer_size(&keypad_rb) == 0) {
		/* wait for key press */
		// Espera a que se presione una tecla y la coloca
		//en el buffer circular (ring buffer)
		uint8_t key_pressed = keypad_run(&keypad_event);
		if (key_pressed != KEY_PRESSED_NONE) {
			ring_buffer_put(&keypad_rb, key_pressed);
		}
	}
	// Una vez que se ha presionado una tecla, obtén la tecla del buffer circular
	uint8_t key_pressed;
	ring_buffer_get(&keypad_rb, &key_pressed);
	// Verifica si la tecla presionada es '*' o '#'. Si es así, devuelve 0xFF.
	if (key_pressed == '*' || key_pressed == '#') {
		return 0xFF;
	}
    // Devuelve la tecla presionada
	return key_pressed;
}

static uint8_t lock_get_password(void)
{
	uint8_t idx = 0;
	uint8_t passkey = 0;
	uint8_t new_password[MAX_PASSWORD];
	memset(new_password, 0, MAX_PASSWORD);
	uint8_t password_shadow[MAX_PASSWORD + 1]  = {
			'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '\0'
	};
	while (passkey != 0xFF) {
		GUI_update_password(password_shadow);
		passkey = lock_get_passkey();
		password_shadow[idx] = '*';
		new_password[idx++] = passkey;
		GUI_update_password(new_password);
		HAL_Delay(200);
	}

	if (idx > 1) {
		memcpy(password, new_password, MAX_PASSWORD);
		GUI_update_password_success();
		//GUI_Menu();
	} else {
		GUI_locked();

		return 0;
	}
	return 1;
}
//Función lock_validate_password:
//Esta función parece validar la contraseña ingresada comparándola con la secuencia de teclas almacenadas en el búfer circular (keypad_rb).
//Se utiliza ring_buffer_get para obtener la secuencia almacenada en el búfer.
//Se compara la secuencia con la contraseña almacenada en la variable password.
//Si la comparación es exitosa, la función retorna 1; de lo contrario, retorna 0.
static uint8_t lock_validate_password(void)
{
	uint8_t sequence[MAX_PASSWORD];
	uint8_t seq_len = ring_buffer_size(&keypad_rb);
	for (uint8_t idx = 0; idx < seq_len; idx++) {
		ring_buffer_get(&keypad_rb, &sequence[idx]);
	}
	if (memcmp(sequence, password, 6) == 0) {
		return 1;
	}
	return 0;
}
//Función lock_update_password:
//Esta función actualiza la pantalla OLED para solicitar una nueva contraseña utilizando GUI_update_password_init().
//Luego llama a lock_get_password() para obtener la nueva contraseña.
//Si la contraseña se ha ingresado correctamente, se enciende un LED con Encender_LED(). Si no, se llama a GUI_locked() y se apaga el LED con Apagar_LED().
static void lock_update_password(void)
{
	if (lock_validate_password() != 0) {
		GUI_update_password_init();
		lock_get_password();

	} else {
		GUI_locked();

		Apagar_LED();
	}
}
//Función lock_open_lock:
//Esta función parece estar relacionada con la apertura del sistema bloqueado.
//Si la contraseña es válida, se muestra un menú utilizando GUI_Menu() y se enciende un LED.
//Se obtiene la tecla presionada mediante keypad_run y se retorna 1 si es válida.
//Si la contraseña no es válida, se llama a GUI_locked(), se apaga el LED y se retorna 0.
static uint8_t lock_open_lock(void)
{
	uint8_t temp=0;
	if (lock_validate_password() != 0) {
		GUI_Menu();
		Encender_LED();
//		temp=lock_get_passkey();
//		if (temp == '#'){
			uint8_t key_pressed = keypad_run(&keypad_event);
//			if (key_pressed != KEY_PRESSED_NONE) {
//				  lock_sequence_handler(key_pressed);

		return 1;
	}
	else {

		GUI_locked();

		Apagar_LED();
		return 0;
	}
}

void lock_init(void)
{
	ring_buffer_init(&keypad_rb, keypad_buffer, 12);
	GUI_init();
}
void lock_keylock(uint8_t key){
	while((key == '1')||(key == '2')||(key == '3')){
		if(key == '1'){
			ssd1306_SetCursor(10, 25);
			ssd1306_WriteString("Bienvenido", Font_6x8, White);
			ssd1306_DrawBitmap(50, 35, carita, 30, 30, White);
			HAL_Delay(1000);
		}
		else if(key =='2'){
			ssd1306_SetCursor(10, 25);
			ssd1306_WriteString("HOLA", Font_16x26, White);
			ssd1306_DrawBitmap(50, 35, mano, 30, 30, White);
			HAL_Delay(1000);
		}
		else if(key =='3'){
			ssd1306_SetCursor(10, 25);
			ssd1306_WriteString("BIEN", Font_6x8, White);
			ssd1306_DrawBitmap(50, 35, bien, 30, 30, White);
			HAL_Delay(1000);


		}
	}
}
/*void lock_sequence_handler(uint8_t key)
{
	if (key == '1') {
		// Handle option 1
		ssd1306_SetCursor(20, 25);
	    ssd1306_WriteString("Bienvenido", Font_6x8, White);
		// Add your code for option 1 here
	} else if (key == '2') {
		// Handle option 2
		ssd1306_SetCursor(10, 25);
		ssd1306_WriteString("consultar saldo", Font_6x8, White);
		// Add your code for option 2 here
	} else if (key == '3') {
		// Handle option 3
		ssd1306_SetCursor(10, 25);
		ssd1306_WriteString("Retirar dinero", Font_6x8, White);
		// Add your code for option 3 here
	} else if (key == '*') {
		lock_update_password();
	} else if (key == '#') {
		lock_open_lock();
	} else {
		ring_buffer_put(&keypad_rb, key);

	}

}*/
//static uint8_t current_option = 0;
//Función lock_sequence_handler:
//Esta función maneja las teclas ingresadas.
//Si se presiona '*', se llama a lock_update_password().
//Si se presiona '#', se llama a lock_open_lock().
//Si se presiona otra tecla, se almacena en el búfer circular.
void lock_sequence_handler(uint8_t key){
    /*if (key >= '1' && key <= '3') {
        // Cambiar a la opción seleccionada
        current_option = key - '0';

        // Agregar código específico para cada opción si es necesario
        if (current_option == 1) {
            // Código para la opción 1
            ssd1306_SetCursor(10, 25);
            ssd1306_WriteString("Bienvenido", Font_6x8, White);
        } else if (current_option == 2) {
            // Código para la opción 2
            ssd1306_SetCursor(10, 25);
            ssd1306_WriteString("Consultar saldo", Font_6x8, White);
        } else if (current_option == 3) {
            // Código para la opción 3
            ssd1306_SetCursor(10, 25);
            ssd1306_WriteString("Retirar dinero", Font_6x8, White);
        }
    } */ if (key == '*') {
        lock_update_password();
    } else if (key == '#') {

    	logged = lock_open_lock();

    	if (logged == 1){

    	}
    } else {
        // Manejar otras teclas o almacenar en el buffer, según sea necesario
        ring_buffer_put(&keypad_rb, key);
    }
}


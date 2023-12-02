#include "gui.h"

#include "ssd1306.h"
#include "ssd1306_fonts.h"


//para agregar imagenes a nuestro display en este tipo de funciones ya que son importadas
//se agregaron un bien, una carita y una mano
extern const uint8_t locked[];
extern const uint8_t carita[];
extern const uint8_t mano[];
extern const uint8_t bien[];

void GUI_init(void)
{
	ssd1306_Init();
	GUI_locked();


}
//toda esta funcion nos indica nuestro Menu al ingresar la contraseña correcta
void GUI_Menu(void)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(20, 5);
	ssd1306_WriteString("Menu", Font_7x10, White);
	//Muestra las opciones del menú
		    ssd1306_SetCursor(30, 25);
		    ssd1306_WriteString("1.Opcion 1", Font_6x8, White);

		    ssd1306_SetCursor(30, 40);
		    ssd1306_WriteString("2.Opcion 2", Font_6x8, White);

		    ssd1306_SetCursor(30, 55);
		    ssd1306_WriteString("3.Opcion 3", Font_6x8, White);
		//ssd1306_DrawBitmap(50, 35, locked, 30, 30, White);
		    //ssd1306_DrawBitmap(50, 35, carita, 30, 30, White);
		ssd1306_UpdateScreen();
	}
// para que inicialmente me muestre el bloqueado
void GUI_locked(void)
{
	/// Llena toda la pantalla de negro para indicar que el sistema está bloqueado.
	ssd1306_Fill(Black);
	// Establece la posición del cursor en la pantalla y escribe el texto "Locked" en blanco.
	ssd1306_SetCursor(45, 5);
	ssd1306_WriteString("Locked", Font_7x10, White);
	// Establece otra posición del cursor y escribe el texto "enter password:" en blanco.
	ssd1306_SetCursor(10, 25);
	//un aviso lijero de que si nunca ingresa la contraseña no abrila el menu
	ssd1306_WriteString("enter password:", Font_7x10, White);



	// Muestra las opciones del menú
	    //ssd1306_SetCursor(30, 25);
	    //ssd1306_WriteString("1.Opcion 1", Font_6x8, White);

	    //ssd1306_SetCursor(30, 40);
	    //ssd1306_WriteString("2.Opcion 2", Font_6x8, White);

	    //ssd1306_SetCursor(30, 55);
	    //ssd1306_WriteString("3.Opcion 3", Font_6x8, White);
	ssd1306_DrawBitmap(50, 35, locked, 30, 30, White);
	ssd1306_UpdateScreen();
}

void GUI_unlocked(void)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(20, 5);
	ssd1306_WriteString("Unlocked", Font_7x10, White);
	ssd1306_UpdateScreen();
}

void GUI_update_password_init(void)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(5, 5);
	ssd1306_WriteString("New Password:", Font_7x10, White);
	//// Actualiza la pantalla para que los cambios sean visibles.
	ssd1306_UpdateScreen();
}

void GUI_update_password(uint8_t *password)
{
	ssd1306_SetCursor(10, 35);
	ssd1306_WriteString((char *)password, Font_7x10, White);
	ssd1306_UpdateScreen();
}

void GUI_update_password_success(void)
{
	ssd1306_SetCursor(5, 35);
	ssd1306_WriteString("Success!", Font_7x10, White);
	// se le agrego que aparezca una carita feliz alado de success
	ssd1306_DrawBitmap(60, 35, carita, 30, 30, White);
	ssd1306_UpdateScreen();
}


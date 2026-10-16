# dinocpp - Dino Offline (C++ + raylib)

Clon del juego del dinosaurio de Chrome cuando no hay internet, hecho en C++ con raylib. Todo se dibuja con formas, sin imagenes.

## Jugar (sin compilar)

1. Ve a la pagina de [Releases](https://github.com/itsdididev/dinocpp/releases/latest).
2. Descarga `dino.exe` (Windows 64 bits).
3. Haz doble clic para abrirlo. No necesita instalar nada.

Si Windows SmartScreen avisa, pulsa "Mas informacion" y luego "Ejecutar de todas formas".

## Controles

| Tecla | Accion |
|-------|--------|
| Espacio / Flecha arriba | Saltar |
| Flecha abajo | Agacharse (o caer mas rapido en el aire) |
| Espacio / Enter | Reiniciar tras perder |

## Compilar desde el codigo

Necesitas CMake y un compilador de C++ (Visual Studio o MinGW). La primera vez se necesita internet para que CMake descargue raylib.

    cmake -S . -B build
    cmake --build build --config Release

El ejecutable queda en `build\Release\dino.exe` (con MinGW, en `build\dino.exe`).

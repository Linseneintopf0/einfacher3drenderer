## Sourcecode des praktischen Teils meiner Facharbeit
Das ist der Sourcecode meiner Facharebit. Er verwendet die Windows API (Win32 API) und DirectX 11 um 3D Objekte darzustellen.

### Verwendnung
In WinMain.cpp werden Fenster geöffnet, durch erstellen eines raw pointers zu einem Objekt der "Window" Klasse. Der Konstruktor der Klasse verlangt Höhe, Breite, ein "Color" Objekt Pointer und ein String mit einem gültigen Dateipfad für das Program ODER ein Keyword für vordefiniertes Verhalten.

Ein Color Objekt muss definiert werden mit Werten zwischen 0 und 1 für die Rot, Grün und Blau Werte der Farbe und ein Alpha Wert, der auch zwischen 0 und 1 liegt.

```cpp
#include color.h
#include window.h

WinMain(
//...
)
{
Color white(1, 1, 1, 1);

Window* wnd1 = new Window(300, 800, white, "RAINBOW");
//...
}
```

Window Objekte werden beim Schließen des Fensters ```deleted```, um den Speicher zu releasen. Das Program beendet und unregistriert alle ```WindowClass```es beim Schließen des letzten Fensters. Das Program muss nach Änderungen im Quelltest neu kompiliert werden, Compiler Fehler können auftreten, ebenfalls Fehler mit DX11, abhängig von der Hardware.

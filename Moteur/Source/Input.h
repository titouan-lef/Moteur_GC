#include <bitset>
#include <Windows.h>

class Input
{
public:
    Input();

    // Met à jour l'état du clavier à chaque trame
    void Update();


    // Vérifie si une touche est enfoncée
    bool KeyIsPressed(unsigned char keycode) const;

    // Vérifie si une touche a été enfoncée depuis la dernière mise à jour
    bool KeyIsPressedOnce(unsigned char keycode) const;

    // Vérifie si une touche a été relâchée depuis la dernière mise à jour
    bool KeyIsReleased(unsigned char keycode) const;

private:
    // Utilisez un std::bitset pour stocker l'état de chaque touche
    std::bitset<256> keystates;
    std::bitset<256> prevKeystates;
    POINT* m_mouse;
};

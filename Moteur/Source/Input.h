#include <bitset>
#include <Windows.h>

class Input
{
public:
    Input();

    // Met � jour l'�tat du clavier � chaque trame
    void Update();


    // V�rifie si une touche est enfonc�e
    bool KeyIsPressed(unsigned char keycode) const;

    // V�rifie si une touche a �t� enfonc�e depuis la derni�re mise � jour
    bool KeyIsPressedOnce(unsigned char keycode) const;

    // V�rifie si une touche a �t� rel�ch�e depuis la derni�re mise � jour
    bool KeyIsReleased(unsigned char keycode) const;

private:
    // Utilisez un std::bitset pour stocker l'�tat de chaque touche
    std::bitset<256> keystates;
    std::bitset<256> prevKeystates;
    POINT* m_mouse;
};

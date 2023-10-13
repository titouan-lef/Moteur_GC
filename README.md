# Moteur_GC

## Propriété du projet "Moteur"

- Configuration Propreties --> General --> C++ language Standard --> Preview - ...
- Configuration Propreties --> Linker --> System --> SubSystem --> Windows (/SUBSYSTEM:WINDOWS)
- Configuration Propreties --> Advanced --> Character Set --> Use Multi-Byte Character Set

## Vocabulaire :

- handle : référence permettant de gérer et d'identifier de manière unique des ressources, des objets ou des données
- message : événement Windows

## Convention :

- Ecrire l'utilité des fonctions d'une bibliothèque dans le readme
- Ecrire l'utilité de nos fonction dans le .h
- Expliquer les paramètres de fonctiondans le .cpp
- Les variables de classe s'écrivent nomVariableClasse
- Les paramètres s'écrivent _nomParam

## Fonction Windows.h

### GetModuleHandle(nullptr)
Handle vers notre application

### AdjustWindowRect(...)
Agrandissement de la taille de la fenêtre pour prendre en compte son style et ses bordures

### PeekMessage(...)
Gérer les messages d'action d'une fenêtre

### TranslateMessage(&msg)
Convertie le message à clé virtuelle en messages caractères

### DispatchMessage(&msg)
Transmet le message à une procédure de fenêtre pour réaliser l'action. La procédure est définie dans le champs lpfnWndProc de la WNDCLASS

### PostQuitMessage(0)
Envoie du message WM_QUIT

### DefWindowProc(...)
Réalise le traitement par défaut du message
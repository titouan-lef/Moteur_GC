# Moteur_GC

## Propriété du projet "Moteur"

- Configuration Propreties --> General --> C++ language Standard --> Preview - ...
- Configuration Propreties --> Linker --> System --> SubSystem --> Windows (/SUBSYSTEM:WINDOWS)
- Configuration Propreties --> Advanced --> Character Set --> Use Multi-Byte Character Set

## Vocabulaire :

- handle : référence permettant de gérer et d'identifier de manière unique des ressources, des objets ou des données

## Fonction Windows.h

### AdjustWindowRect(...)

**Utilité** : Agrandissement de la taille de la fenêtre pour prendre en compte son style et ses bordures

**Retour** : void

**Paramètre**
- lpRect : position des 4 coins de la fenêtre
- dwStyle : style et fonctionnalités (ex : réduction) de la fenêtre
- bMenu : activé ou non le menu d'assistance

### CreateWindow(...)

**Utilité** : Créer une fenêtre

**Retour**
- Si la fonction réussit, la valeur de retour est un handle pour la nouvelle fenêtre
- Sinon, la valeur de retour est NULL

**Paramètre**
- lpClassName : nom de la fenêtre
- lpWindowName : nom afficher sur la fenêtre
- dwStyle : style et fonctionnalités (ex : réduction) de la fenêtre
- x et y : coordonées (x, y) du coin supérieur gauche de la fenêtre
- nWidth et nHeight : longueur et hauteur de la fenêtre
- hWndParent : handle de la fenêtre parente
- hMenu : handle d’un menu
- hInstance : handle de l'instance de la fenêtre
- lpParam : message de la fenêtre

### PeekMessage(...)

**Utilité** : Gérer les messages d'action d'une fenêtre

**Retour**
- false si aucun message de fenêtre n'est disponible
- true si au moins un message de disponnible dans la file

**Paramètre**
- lpMsg : reçoit le premier message de la file
- hWnd : handle de la fenêtre dont les messages doivent être récupérés
    - si NULL alors récupère les messages pour toutes les fenêtres du thread
    - si -1 alors récupère uniquement les messages de la file d’attente de messages du thread
- wMsgFilterMin : valeur du premier message dans la plage de messages à examiner
- wMsgFilterMax : valeur du dernier message dans la plage de messages à examiner
- wRemoveMsg : spécifie comment les messages doivent être gérés
    - PM_NOREMOVE : les messages ne sont pas supprimés de la file d’attente après le traitement par PeekMessage
    - PM_REMOVE : les messages sont supprimés de la file d’attente après traitement par PeekMessage
    - PM_NOYIELD : empêche le système de libérer tout thread qui attend que l’appelant soit inactif
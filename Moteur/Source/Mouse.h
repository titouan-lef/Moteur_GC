#pragma once // Directive de pr�processeur pour s'assurer que le fichier est inclus une seule fois

#include <queue> // Inclut l'en-t�te pour utiliser la classe de file (queue)

class Mouse // D�finition de la classe Mouse
{
	friend class Window; // Permet � la classe Window d'acc�der aux membres priv�s de la classe Mouse

public:
	// Classe interne Event pour repr�senter les �v�nements de la souris
	class Event {
	public:
		enum class Type {
			LPress,     // Bouton gauche de la souris enfonc�
			LRelease,   // Bouton gauche de la souris rel�ch�
			RPress,     // Bouton droit de la souris enfonc�
			RRelease,   // Bouton droit de la souris rel�ch�
			WheelUp,    // Molette de la souris vers le haut
			WheelDown,  // Molette de la souris vers le bas
			Enter,      // Curseur de la souris entre dans la fen�tre
			Leave,      // Curseur de la souris quitte la fen�tre
			Move,       // Mouvement de la souris
			Invalid     // Type d'�v�nement invalide
		};

	private:
		Type type;              // Type de l'�v�nement de la souris
		bool leftIsPressed;    // Indique si le bouton gauche de la souris est enfonc�
		bool rightIsPressed;   // Indique si le bouton droit de la souris est enfonc�
		int x;                  // Coordonn�e en x du curseur de la souris
		int y;                  // Coordonn�e en y du curseur de la souris

	public:
		// Constructeur par d�faut de l'�v�nement de la souris
		Event() noexcept : type(Type::Invalid), leftIsPressed(false), rightIsPressed(false), x(0), y(0) {}

		// Constructeur de l'�v�nement de la souris avec type et �tat de boutons
		Event(Type type, const Mouse& parent) noexcept : type(type), leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed), x(parent.x), y(parent.y) {}

		// V�rifie si l'�v�nement de la souris est valide
		bool IsValid() const noexcept {
			return type != Type::Invalid;
		}

		// Obtient le type de l'�v�nement de la souris
		Type GetType() const noexcept {
			return type;
		}

		// Obtient les coordonn�es du curseur de la souris sous forme de paire (x, y)
		std::pair<int, int> GetPos() const noexcept {
			return { x, y };
		}

		// Obtient la coordonn�e x du curseur de la souris
		int GetPosX() const noexcept {
			return x;
		}

		// Obtient la coordonn�e y du curseur de la souris
		int GetPosY() const noexcept {
			return y;
		}

		// V�rifie si le bouton gauche de la souris est enfonc�
		bool LeftIsPressed() const noexcept {
			return leftIsPressed;
		}

		// V�rifie si le bouton droit de la souris est enfonc�
		bool RightIsPressed() const noexcept {
			return rightIsPressed;
		}
	};

public:
	// Constructeur par d�faut de la classe Mouse
	Mouse() = default;

	// D�sactive la copie et l'affectation par copie de la classe Mouse
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	// Obtient les coordonn�es du curseur de la souris sous forme de paire (x, y)
	std::pair<int, int> GetPos() const noexcept;

	// Obtient la coordonn�e x du curseur de la souris
	int GetPosX() const noexcept;

	// Obtient la coordonn�e y du curseur de la souris
	int GetPosY() const noexcept;

	// V�rifie si le curseur de la souris est � l'int�rieur de la fen�tre
	bool IsInWindow() const noexcept;

	// V�rifie si le bouton gauche de la souris est enfonc�
	bool LeftIsPressed() const noexcept;

	// V�rifie si le bouton droit de la souris est enfonc�
	bool RightIsPressed() const noexcept;

	// Lit et renvoie le prochain �v�nement de la souris dans la file d'�v�nements
	Mouse::Event Read() noexcept;

	// V�rifie si la file d'�v�nements de la souris est vide
	bool IsEmpty() const noexcept {
		return buffer.empty();
	}

	// Vide la file d'�v�nements de la souris
	void Flush() noexcept;

private:
	// M�thode pour g�rer le mouvement de la souris
	void OnMouseMove(int newx, int newy) noexcept;

	// M�thode pour g�rer la sortie du curseur de la souris de la fen�tre
	void OnMouseLeave() noexcept;

	// M�thode pour g�rer l'entr�e du curseur de la souris dans la fen�tre
	void OnMouseEnter() noexcept;

	// M�thodes pour g�rer le clic du bouton gauche de la souris
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;

	// M�thodes pour g�rer le clic du bouton droit de la souris
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;

	// M�thodes pour g�rer le d�filement de la souris
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;

	// M�thode pour r�duire la taille de la file d'�v�nements
	void TrimBuffer() noexcept;

private:
	static constexpr unsigned int bufferSize = 16u; // Taille maximale de la file d'�v�nements
	int x;                                           // Coordonn�e en x du curseur de la souris
	int y;                                           // Coordonn�e en y du curseur de la souris
	bool leftIsPressed = false;                      // Indique si le bouton gauche de la souris est enfonc�
	bool rightIsPressed = false;                     // Indique si le bouton droit de la souris est enfonc�
	bool isInWindow = false;                        // Indique si le curseur de la souris est � l'int�rieur de la fen�tre
	int wheelDeltaCarry = 0;                        // Valeur de d�filement accumul�e pour la molette de la souris
	std::queue<Event> buffer;                      // File d'�v�nements de la souris
};

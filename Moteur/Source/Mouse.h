#pragma once // Directive de préprocesseur pour s'assurer que le fichier est inclus une seule fois

#include <queue> // Inclut l'en-tête pour utiliser la classe de file (queue)

class Mouse // Définition de la classe Mouse
{
	friend class Window; // Permet à la classe Window d'accéder aux membres privés de la classe Mouse

public:
	// Classe interne Event pour représenter les événements de la souris
	class Event {
	public:
		enum class Type {
			LPress,     // Bouton gauche de la souris enfoncé
			LRelease,   // Bouton gauche de la souris relâché
			RPress,     // Bouton droit de la souris enfoncé
			RRelease,   // Bouton droit de la souris relâché
			WheelUp,    // Molette de la souris vers le haut
			WheelDown,  // Molette de la souris vers le bas
			Enter,      // Curseur de la souris entre dans la fenêtre
			Leave,      // Curseur de la souris quitte la fenêtre
			Move,       // Mouvement de la souris
			Invalid     // Type d'événement invalide
		};

	private:
		Type type;              // Type de l'événement de la souris
		bool leftIsPressed;    // Indique si le bouton gauche de la souris est enfoncé
		bool rightIsPressed;   // Indique si le bouton droit de la souris est enfoncé
		int x;                  // Coordonnée en x du curseur de la souris
		int y;                  // Coordonnée en y du curseur de la souris

	public:
		// Constructeur par défaut de l'événement de la souris
		Event() noexcept : type(Type::Invalid), leftIsPressed(false), rightIsPressed(false), x(0), y(0) {}

		// Constructeur de l'événement de la souris avec type et état de boutons
		Event(Type type, const Mouse& parent) noexcept : type(type), leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed), x(parent.x), y(parent.y) {}

		// Vérifie si l'événement de la souris est valide
		bool IsValid() const noexcept {
			return type != Type::Invalid;
		}

		// Obtient le type de l'événement de la souris
		Type GetType() const noexcept {
			return type;
		}

		// Obtient les coordonnées du curseur de la souris sous forme de paire (x, y)
		std::pair<int, int> GetPos() const noexcept {
			return { x, y };
		}

		// Obtient la coordonnée x du curseur de la souris
		int GetPosX() const noexcept {
			return x;
		}

		// Obtient la coordonnée y du curseur de la souris
		int GetPosY() const noexcept {
			return y;
		}

		// Vérifie si le bouton gauche de la souris est enfoncé
		bool LeftIsPressed() const noexcept {
			return leftIsPressed;
		}

		// Vérifie si le bouton droit de la souris est enfoncé
		bool RightIsPressed() const noexcept {
			return rightIsPressed;
		}
	};

public:
	// Constructeur par défaut de la classe Mouse
	Mouse() = default;

	// Désactive la copie et l'affectation par copie de la classe Mouse
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	// Obtient les coordonnées du curseur de la souris sous forme de paire (x, y)
	std::pair<int, int> GetPos() const noexcept;

	// Obtient la coordonnée x du curseur de la souris
	int GetPosX() const noexcept;

	// Obtient la coordonnée y du curseur de la souris
	int GetPosY() const noexcept;

	// Vérifie si le curseur de la souris est à l'intérieur de la fenêtre
	bool IsInWindow() const noexcept;

	// Vérifie si le bouton gauche de la souris est enfoncé
	bool LeftIsPressed() const noexcept;

	// Vérifie si le bouton droit de la souris est enfoncé
	bool RightIsPressed() const noexcept;

	// Lit et renvoie le prochain événement de la souris dans la file d'événements
	Mouse::Event Read() noexcept;

	// Vérifie si la file d'événements de la souris est vide
	bool IsEmpty() const noexcept {
		return buffer.empty();
	}

	// Vide la file d'événements de la souris
	void Flush() noexcept;

private:
	// Méthode pour gérer le mouvement de la souris
	void OnMouseMove(int newx, int newy) noexcept;

	// Méthode pour gérer la sortie du curseur de la souris de la fenêtre
	void OnMouseLeave() noexcept;

	// Méthode pour gérer l'entrée du curseur de la souris dans la fenêtre
	void OnMouseEnter() noexcept;

	// Méthodes pour gérer le clic du bouton gauche de la souris
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;

	// Méthodes pour gérer le clic du bouton droit de la souris
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;

	// Méthodes pour gérer le défilement de la souris
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;

	// Méthode pour réduire la taille de la file d'événements
	void TrimBuffer() noexcept;

private:
	static constexpr unsigned int bufferSize = 16u; // Taille maximale de la file d'événements
	int x;                                           // Coordonnée en x du curseur de la souris
	int y;                                           // Coordonnée en y du curseur de la souris
	bool leftIsPressed = false;                      // Indique si le bouton gauche de la souris est enfoncé
	bool rightIsPressed = false;                     // Indique si le bouton droit de la souris est enfoncé
	bool isInWindow = false;                        // Indique si le curseur de la souris est à l'intérieur de la fenêtre
	int wheelDeltaCarry = 0;                        // Valeur de défilement accumulée pour la molette de la souris
	std::queue<Event> buffer;                      // File d'événements de la souris
};

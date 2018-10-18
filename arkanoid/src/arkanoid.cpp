// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>

#include <cmath>
#include <algorithm>
#include <vector>
#include <functional>

using namespace std;

bool isKeyPressed[1024];
function<void()> callbacks[1024];
void setCallback(int key, function<void()> callback)
{
    callbacks[key] = callback;
}

void onKeyClicked(GLFWwindow* , int key, int , int action, int)
{
    if (key >= 0 && key < 1024)
    {
        bool firstPressed = !isKeyPressed[key] && action == GLFW_PRESS;
        if (action == GLFW_PRESS)
        {
            isKeyPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            isKeyPressed[key] = false;
        }
        if(firstPressed && callbacks[key])
        {
            callbacks[key]();
        }
    }
}

bool initWindow()
{
    // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Memeory", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profilefile:///home/i291481/Pobrane/memory/memory.vs

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return false;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(window, onKeyClicked);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return true;
}

GLuint loadShaderProgram()
{
    return LoadShaders( "memory.vs", "memory.fs" );
}


GLuint translationId;
GLuint scaleId;
GLuint colorId;
GLuint timeId;
GLuint timeScaleImpactId;


struct DrawObject
{
    int from;
    int length;
    GLenum mode;
    float timeScaleImpact;

    bool operator==(const DrawObject& other)
    {
        return from == other.from && length == other.length && mode == other.mode;
    }

    void draw()
    {
        glUniform1f(timeScaleImpactId, timeScaleImpact);
        glDrawArrays(mode, from, length);
    }

};

const GLfloat bufferData[] = {
    // card
    -0.8f, -0.8f,
    0.8f, -0.8f,
    0.8f, 0.8f,
    -0.8f, 0.8f,

    //frame
    -0.9f, -0.9f,
    0.9f, -0.9f,
    0.9f, 0.9f,
    -0.9f, 0.9f,

    //line
    -0.7f, -0.7f,
    0.7f, 0.7f,

    //cross
    -0.5f, 0.3f,
    0.5f, 0.3,
    0.0f, 0.6f,
    0.0f, -0.6f,

    //triangle
    -0.6f, -0.6f,
    0.6f, -0.6f,
    0.0f, 0.6f
};

DrawObject cardBackground = {0, 4, GL_TRIANGLE_FAN, 0.0f};
DrawObject cardFrame = {4, 4, GL_TRIANGLE_FAN, 0.1f};
DrawObject line = {8, 2, GL_LINES, 0.05f};
DrawObject cross = {10, 4, GL_LINES, 0.05f};
DrawObject triangle = {14, 3, GL_LINE_LOOP, 0.05f};


struct Color
{
    float r;
    float g;
    float b;

    bool operator==(const Color& other)
    {
        return r == other.r && g == other.g && b == other.b;
    }
};

constexpr Color WHITE = {1.0f, 1.0f, 1.0f};
constexpr Color BLACK = {0.0f, 0.0f, 0.0f};
constexpr Color GREY = {0.75f, 0.75f, 0.75f};
constexpr Color BROWN = {1.0f, 0.9f, 0.8f};

constexpr Color RED = {1.0f, 0.0f, 0.0f};
constexpr Color GREEN = {0.0f, 1.0f, 0.0f};
constexpr Color BLUE = {0.0f, 0.0f, 1.0f};
constexpr Color YELLOW = {1.0f, 1.0f, 0.0f};
constexpr Color MAGENTA = {1.0f, 0.0f, 1.0f};
constexpr Color CYAN = {0.0f, 1.0f, 1.0f};

void setColor(const Color& color)
{
    glUniform3f(colorId, color.r, color.g, color.b);
}

struct Card
{
    DrawObject sign;
    pair<float,float> translation = {0.0f, 0.0f};
    pair<float,float> scale = {1.0f, 1.0f};
    Color color;
    static const Color FRAME_COLOR;
    static const Color SIGN_COLOR;
    static const Color BACK_COLOR;

    bool isSelected = false;
    bool showFront = false;
    bool isCollected = false;

    Card(){}
    Card(DrawObject sign, Color color)
        : sign(sign), color(color)
    {
    }

    bool operator==(const Card& card)
    {
        return sign == card.sign && color == card.color;
    }

    void setTranslation(pair<float,float> translation)
    {
        this->translation = translation;
    }

    void setScale(pair<float,float> scale)
    {
        this->scale = scale;
    }

    void setSelection(bool isSelected)
    {
        this->isSelected = isSelected;
    }

    void flip()
    {
        showFront = !showFront;
    }

    void collect()
    {
        isCollected = true;
    }

    bool isShowed()
    {
        return !isCollected && showFront;
    }

    void draw()
    {
        if(isCollected)
        {
            return;
        }

        glUniform2f(translationId, translation.first, translation.second);
		glUniform2f(scaleId, scale.first, scale.second);
        if(isSelected)
        {
            setColor(FRAME_COLOR);
	        cardFrame.draw();
        }
		if(showFront)
        {
            setColor(color);
            cardBackground.draw();

            setColor(SIGN_COLOR);
            sign.draw();
        }
        else
        {
            setColor(BACK_COLOR);
            cardBackground.draw();
        }
    }
};
const Color Card::FRAME_COLOR = GREY;
const Color Card::SIGN_COLOR = BLACK;
const Color Card::BACK_COLOR = BROWN;

struct Board
{
    vector<vector<Card>> cards;
    int i;
    int j;
    int roundNumber;
    Board(vector<vector<Card>> cards)
    {
        initBoardCards(cards);
    }

    void initBoardCards(vector<vector<Card>> cards)
    {
        this->cards = cards;
        i = j = 0;
        roundNumber = 1;
        this->cards[i][j].setSelection(true);
        showRoundNumber();
    }

    void showRoundNumber()
    {
        printf("Round number %d\n", roundNumber);
    }

    void addToI(int di)
    {
        i = (i + di + cards.size()) % cards.size();
    }

    void addToJ(int dj)
    {
        j = (j + dj + cards[i].size()) % cards[i].size();
    }

    void moveSelectionI(int di)
    {
        int startI = i;
        int startJ = j;
        cards[i][j].setSelection(false);
        addToI(di);
        while(cards[i][j].isCollected && (startI != i || startJ != j))
        {
            if(i + di >= static_cast<int>(cards.size()) || i + di < 0)
            {
                addToJ(1);
            }
            addToI(di);
        }
        cards[i][j].setSelection(true);
    }

    void moveSelectionJ(int dj)
    {
        int startI = i;
        int startJ = j;
        cards[i][j].setSelection(false);
        addToJ(dj);
        while(cards[i][j].isCollected &&(startI != i || startJ != j))
        {
            if(j + dj >= static_cast<int>(cards[i].size()) || j + dj < 0)
            {
                addToI(1);
            }
            addToJ(dj);
        }
        cards[i][j].setSelection(true);
    }

    vector<Card*> findShowedCards()
    {
        vector<Card*> showedCards;
        for(auto& row : cards)
        {
            for(auto& card : row)
            {
                if(card.isShowed())
                {
                    showedCards.push_back(&card);
                }
            }
        }
        return showedCards;
    }

    bool allCardsCollected()
    {
        for(auto& row : cards)
        {
            for(auto& card : row)
            {
                if(!card.isCollected)
                {
                    return false;
                }
            }
        }
        return true;
    }

    void flipCard()
    {
        if(cards[i][j].isShowed())
        {
            return;
        }

        cards[i][j].flip();
        auto showedCards = findShowedCards();
        if(showedCards.size() == 2)
        {
            if(*showedCards[0] == *showedCards[1])
            {
                showedCards[0]->collect();
                showedCards[1]->collect();
                moveSelectionJ(1);
            }
            else
            {
                showedCards[0]->flip();
                showedCards[1]->flip();
                cards[i][j].flip();
            }
            roundNumber++;
            showRoundNumber();
        }
    }

    void draw()
    {
        for(auto& row : cards)
        {
            for(auto& card : row)
            {
                card.draw();
            }
        }
    }
};

GLuint programId;
void initShaderProgram()
{
    programId = loadShaderProgram();
    glUseProgram(programId);

	translationId = glGetUniformLocation(programId, "translation");
    scaleId = glGetUniformLocation(programId, "scale");
    colorId = glGetUniformLocation(programId, "color");
    timeId = glGetUniformLocation(programId, "time");
    timeScaleImpactId = glGetUniformLocation(programId, "timeScaleImpact");
}

GLuint vertaxArrayIdx;
GLuint vertexBufferIdx;
void initDrawData()
{
	glGenVertexArrays(1, &vertaxArrayIdx);
	glBindVertexArray(vertaxArrayIdx);

	glGenBuffers(1, &vertexBufferIdx);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );
}

void clearDrawData()
{
    glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &vertexBufferIdx);
	glDeleteProgram(programId);
	glDeleteVertexArrays(1, &vertaxArrayIdx);
}

vector<Card> createCards(int colorsNumber)
{
    static const vector<DrawObject> signs = {line, cross, triangle};
    vector<Color> cardColors = {RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN};
    while(colorsNumber > static_cast<int>(cardColors.size()))
    {
        cardColors.pop_back();
    }

    vector<Card> cards;
    for(auto& sign : signs)
    {
        for(auto& color : cardColors)
        {
            cards.push_back(Card(sign, color));
        }
    }
    return cards;
}

vector<vector<Card>> createBoardCards(int rows, int columns, int colorsNumber)
{
    int requiredCards = rows * columns;
    vector<Card> cards = createCards(colorsNumber);
    random_shuffle(cards.begin(), cards.end());

    vector<Card> cardsToChoose;
    while(requiredCards > 0)
    {
        cardsToChoose.push_back(cards.back());
        cardsToChoose.push_back(cards.back());
        cards.pop_back();
        requiredCards-=2;
    }
    random_shuffle(cardsToChoose.begin(), cardsToChoose.end());

    vector<vector<Card>> boardCards(rows, vector<Card>(columns));
    float shiftX = 2.0f / (1 + columns);
    float shiftY = 2.0f / (1 + rows);
    pair<float,float> scale = {1.0f/(1+columns), 1.0f/(1+rows)};

    float y = -1.0f;
    for(auto& row : boardCards)
    {
        y += shiftY;
        float x = -1.0f;
        for(auto& card : row)
        {
            x += shiftX;
            card = cardsToChoose.back();
            card.setTranslation({x,y});
            card.setScale(scale);
            cardsToChoose.pop_back();
        }
    }
    return boardCards;
}

void initInputHandlers(Board& board)
{
    setCallback(GLFW_KEY_UP, bind(&Board::moveSelectionI, ref(board), 1));
    setCallback(GLFW_KEY_DOWN, bind(&Board::moveSelectionI, ref(board), -1));
    setCallback(GLFW_KEY_LEFT, bind(&Board::moveSelectionJ, ref(board), -1));
    setCallback(GLFW_KEY_RIGHT, bind(&Board::moveSelectionJ, ref(board), 1));
    setCallback(GLFW_KEY_SPACE, bind(&Board::flipCard, ref(board)));
}

int main(int argc, char* argv[])
{
    if(!initWindow())
    {
        return -1;
    }

    initShaderProgram();
    initDrawData();

    int colorsNumber = 3;
    int rows = 3;
    int columns = 4;

    if(argc > 1)
    {
        colorsNumber = atoi(argv[1]);
    }

    if(argc > 3)
    {
        rows = atoi(argv[2]);
        columns = atoi(argv[3]);
    }

    Board board(createBoardCards(rows, columns, colorsNumber));
    initInputHandlers(board);

    float time = 0.0f;
    const float DELTA_TIME = 0.08f;
	do
    {
        glUniform1f(timeId, time);

        glClear( GL_COLOR_BUFFER_BIT );

        board.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

        if(board.allCardsCollected())
        {
            board.initBoardCards(createBoardCards(rows, columns, colorsNumber));
        }
        time += DELTA_TIME;

	}
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );


    clearDrawData();
	glfwTerminate();

	return 0;
}



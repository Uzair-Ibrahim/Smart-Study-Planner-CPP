#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iomanip>
using namespace std;

class PriorityQueue;
class Subject {
public:
    string name;
    int difficulty;
    int days_left;
    int hours;
    int priority;
    Subject* next;
    Subject(string n, int d, int dl, int h) {
        name = n;
        difficulty = d;
        days_left = dl;
        hours = h;
        priority = difficulty * 2 + (30 - days_left);
        next = nullptr;
    }
    void updatePriority() {
        priority = difficulty * 2 + (30 - days_left);
    }
};

class SubjectList {
public:
    Subject* head;
    SubjectList() { head = nullptr; }
    ~SubjectList() {
        while (head) {
            Subject* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void addSubject(string n, int d, int dl, int h) {
        Subject* newSub = new Subject(n, d, dl, h);
        if (!head) head = newSub;
        else {
            Subject* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newSub;
        }
    }
    vector<string> getSubjectsDataForDisplay();
    Subject* findSubject(string n) {
        Subject* temp = head;
        while (temp) {
            if (temp->name == n) return temp;
            temp = temp->next;
        }
        return nullptr;
    }
    bool removeSubject(string n) {
        if (!head) return false;
        if (head->name == n) {
            Subject* temp = head;
            head = head->next;
            delete temp;
            return true;
        }
        Subject* temp = head;
        while (temp->next && temp->next->name != n) temp = temp->next;
        if (temp->next) {
            Subject* del = temp->next;
            temp->next = temp->next->next;
            delete del;
            return true;
        }
        return false;
    }
};

class StackNode {
public:
    string action;
    Subject* subject;
    StackNode* next;
    StackNode(string a, Subject* s) {
        action = a;
        subject = s;
        next = nullptr;
    }
};

class Stack {
public:
    StackNode* top;
    Stack() { top = nullptr; }
    ~Stack() {
        while (top) {
            StackNode* temp = top;
            top = top->next;
            delete temp->subject;
            delete temp;
        }
    }
    void push(string a, Subject* s) {
        StackNode* newNode = new StackNode(a, s);
        newNode->next = top;
        top = newNode;
    }
    StackNode* pop() {
        if (!top) return nullptr;
        StackNode* temp = top;
        top = top->next;
        return temp;
    }
    bool isEmpty() { return top == nullptr; }
};

class PriorityQueue {
public:
    Subject* head;
    PriorityQueue() { head = nullptr; }
    ~PriorityQueue() {
        while (head) {
            Subject* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void insert(Subject* s) {
        s->next = nullptr;
        if (!head || s->priority > head->priority) {
            s->next = head;
            head = s;
            return;
        }
        Subject* temp = head;
        while (temp->next && temp->next->priority >= s->priority) {
            temp = temp->next;
        }
        s->next = temp->next;
        temp->next = s;
    }
    Subject* extractMax() {
        if (!head) return nullptr;
        Subject* maxSub = head;
        head = head->next;
        maxSub->next = nullptr;
        return maxSub;
    }
    bool isEmpty() { return head == nullptr; }
};

vector<string> SubjectList::getSubjectsDataForDisplay() {
    vector<string> displayData;
    if (!head) {
        displayData.push_back("No subjects added.");
        return displayData;
    }
    PriorityQueue pq;
    Subject* temp = head;
    while (temp) {
        pq.insert(new Subject(temp->name, temp->difficulty, temp->days_left, temp->hours));
        temp = temp->next;
    }
    displayData.push_back(string("Name") + "\t" + "Difficulty" + "\t" + "Days Left" + "\t" + "Hours" + "\t" + "Priority");
    displayData.push_back("----------------------------------------------------------------------------------");
    int rank = 1;
    Subject* s;
    while ((s = pq.extractMax())) {
        stringstream ss;
        string suffix;
        if (rank == 1) suffix = "st";
        else if (rank == 2) suffix = "nd";
        else if (rank == 3) suffix = "rd";
        else suffix = "th";
        ss << left << setw(15) << s->name
           << setw(15) << to_string(s->difficulty)
           << setw(15) << to_string(s->days_left)
           << setw(10) << to_string(s->hours)
           << rank << suffix;
        displayData.push_back(ss.str());
        rank++;
        delete s;
    }
    return displayData;
}

vector<string> generateStudyPlanForDisplay(SubjectList& list) {
    vector<string> displayData;
    if (!list.head) {
        displayData.push_back("No subjects to plan. Add subjects first.");
        return displayData;
    }
    PriorityQueue pq;
    Subject* temp = list.head;
    while (temp) {
        pq.insert(new Subject(temp->name, temp->difficulty, temp->days_left, temp->hours));
        temp = temp->next;
    }
    displayData.push_back("Smart Study Plan (Priority Order)\n");
    int count = 1;
    Subject* s;
    while ((s = pq.extractMax())) {
        string suffix;
        if (count == 1) suffix = "st";
        else if (count == 2) suffix = "nd";
        else if (count == 3) suffix = "rd";
        else suffix = "th";
        stringstream ss;
        ss << count << suffix << ". Study " << s->name
           << " for " << s->hours << " hours (Difficulty: " << s->difficulty
           << ", Days Left: " << s->days_left << ")";
        displayData.push_back(ss.str());
        count++;
        delete s;
    }
    return displayData;
}

class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color idleColor;
    sf::Color hoverColor;
    Button(string t, sf::Vector2f size, int charSize, sf::Color bgColor, sf::Color textColor, sf::Font& font) {
        shape.setSize(size);
        shape.setFillColor(bgColor);
        idleColor = bgColor;
        hoverColor = sf::Color(bgColor.r + 30, bgColor.g + 30, bgColor.b + 30);
        text.setString(t);
        text.setFont(font);
        text.setCharacterSize(charSize);
        text.setFillColor(textColor);
    }
    void setPosition(sf::Vector2f pos) {
        shape.setPosition(pos);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(pos.x + shape.getSize().x / 2.0f, pos.y + shape.getSize().y / 2.0f);
    }
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
    bool isMouseOver(sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        return shape.getGlobalBounds().contains(worldPos);
    }
    void update(sf::RenderWindow& window) {
        if (isMouseOver(window)) {
            shape.setFillColor(hoverColor);
        } else {
            shape.setFillColor(idleColor);
        }
    }
};

class InputField {
public:
    sf::Text label;
    sf::RectangleShape box;
    sf::Text text;
    string inputString;
    bool isActive = false;
    int charLimit;

    InputField(string l, sf::Vector2f size, int limit, sf::Font& font) {
        charLimit = limit;
        label.setString(l);
        label.setFont(font);
        label.setCharacterSize(20);
        label.setFillColor(sf::Color::White);
        box.setSize(size);
        box.setFillColor(sf::Color(50, 50, 50, 150));
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color(100, 100, 100));
        text.setString("");
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
    }
    void setPosition(sf::Vector2f pos) {
        label.setPosition(pos.x, pos.y);
        box.setPosition(pos.x, pos.y + 30);
        text.setPosition(pos.x + 10, pos.y + 35);
    }
    void draw(sf::RenderWindow& window) {
        window.draw(label);
        window.draw(box);
        window.draw(text);
    }
    void handleInput(sf::Event event) {
        if (!isActive) return;
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) {
                if (!inputString.empty()) {
                    inputString.pop_back();
                }
            } else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                if(inputString.length() < charLimit)
                    inputString += static_cast<char>(event.text.unicode);
            }
            text.setString(inputString);
        }
    }
    bool isMouseOver(sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        return box.getGlobalBounds().contains(worldPos);
    }
    void setActive(bool active) {
        isActive = active;
        if (active) {
            box.setOutlineColor(sf::Color::White);
        } else {
            box.setOutlineColor(sf::Color(100, 100, 100));
        }
    }
    void clear() {
        inputString = "";
        text.setString("");
    }
};

enum class AppState {
    MainMenu,
    AddSubject,
    ViewSubjects,
    RemoveSubject,
    EditSubject_Find,
    EditSubject_Edit,
    GeneratePlan
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Smart Study Planner", sf::Style::Close);
    window.setFramerateLimit(60);
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error: Could not load font 'arial.ttf'" << endl;
        return -1;
    }
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.png")) {
        cout << "Error: Could not load background image 'background.png'" << endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        (float)window.getSize().x / backgroundTexture.getSize().x,
        (float)window.getSize().y / backgroundTexture.getSize().y
    );
    SubjectList list;
    Stack undoStack;
    AppState state = AppState::MainMenu;
   
    sf::Text title("Welcome to Smart Study Planner", font, 32);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition(800 / 2.f - title.getGlobalBounds().width / 2.f, 20);
   
    sf::Text statusMessage("", font, 18);
    statusMessage.setFillColor(sf::Color(255, 255, 100));
    statusMessage.setPosition(20, 560);
   
    sf::Color btnColor(70, 70, 130, 200);
    sf::Color backBtnColor(130, 70, 70, 200);
    Button backButton("Back", {150, 50}, 20, backBtnColor, sf::Color::White, font);
    backButton.setPosition({ 325, 500 });
   
    vector<Button> menuButtons;
    menuButtons.push_back(Button("Add Subject", { 300, 50 }, 20, btnColor, sf::Color::White, font));
    menuButtons.push_back(Button("View Subjects", { 300, 50 }, 20, btnColor, sf::Color::White, font));
    menuButtons.push_back(Button("Edit Subject", { 300, 50 }, 20, btnColor, sf::Color::White, font));
    menuButtons.push_back(Button("Remove Subject", { 300, 50 }, 20, btnColor, sf::Color::White, font));
    menuButtons.push_back(Button("Generate Study Plan", { 300, 50 }, 20, btnColor, sf::Color::White, font));
    menuButtons.push_back(Button("Undo Last Action", { 300, 50 }, 20, btnColor, sf::Color::White, font));
    menuButtons.push_back(Button("Exit", { 300, 50 }, 20, backBtnColor, sf::Color::White, font));
    for (int i = 0; i < menuButtons.size(); ++i) {
        menuButtons[i].setPosition({ 250, 100.f + i * 60.f });
    }
   
    vector<InputField> addInputs;
    addInputs.push_back(InputField("Subject Name:", { 300, 40 }, 20, font));
    addInputs.push_back(InputField("Difficulty (1-10):", { 300, 40 }, 2, font));
    addInputs.push_back(InputField("Days Left (1-30):", { 300, 40 }, 2, font));
    addInputs.push_back(InputField("Study Hours (1-24):", { 300, 40 }, 2, font));
    for (int i = 0; i < addInputs.size(); i++) {
        addInputs[i].setPosition({ 250, 100.f + i * 80.f });
    }
   
    Button submitButton("Submit", { 150, 50 }, 20, btnColor, sf::Color::White, font);
    submitButton.setPosition({ 250, 450 });
   
    Button updateButton("Update", { 150, 50 }, 20, btnColor, sf::Color::White, font);
    updateButton.setPosition({ 250, 450 });
   
    InputField findInput("Subject Name:", { 300, 40 }, 20, font);
    findInput.setPosition({ 250, 200 });
   
    Button findButton("Find", { 150, 50 }, 20, btnColor, sf::Color::White, font);
    findButton.setPosition({ 250, 300 });
   
    Button removeButton("Remove", { 150, 50 }, 20, backBtnColor, sf::Color::White, font);
    removeButton.setPosition({ 250, 300 });
   
    vector<sf::Text> displayedLines;
    sf::RectangleShape displayBackground({700, 400});
    displayBackground.setFillColor(sf::Color(0, 0, 0, 150));
    displayBackground.setPosition(50, 100);
    string subjectToEdit = "";
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();    
            if (event.type == sf::Event::TextEntered) {
                if (state == AppState::AddSubject) {
                    for (auto& input : addInputs) input.handleInput(event);
                }
                else if (state == AppState::EditSubject_Edit) {
                    for (size_t i = 1; i < addInputs.size(); ++i) addInputs[i].handleInput(event);
                }
                else if (state == AppState::RemoveSubject || state == AppState::EditSubject_Find) {
                    findInput.handleInput(event);
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (state == AppState::AddSubject) {
                        for(size_t i = 0; i < addInputs.size(); ++i)
                            addInputs[i].setActive(addInputs[i].isMouseOver(window));
                    }
                    else if (state == AppState::EditSubject_Edit) {
                        for (size_t i = 1; i < addInputs.size(); ++i)
                            addInputs[i].setActive(addInputs[i].isMouseOver(window));
                    }
                    else if (state == AppState::RemoveSubject || state == AppState::EditSubject_Find) {
                        findInput.setActive(findInput.isMouseOver(window));
                    }
                    if (state == AppState::MainMenu) {
                        if (menuButtons[0].isMouseOver(window)) {
                            state = AppState::AddSubject;
                            statusMessage.setString("");
                            for(auto& input : addInputs) input.clear();
                        }
                        else if (menuButtons[1].isMouseOver(window)) {
                            state = AppState::ViewSubjects;
                            vector<string> data = list.getSubjectsDataForDisplay();
                            displayedLines.clear();
                            float yOffset = 110;
                            for(const string& line : data) {
                                sf::Text textLine(line, font, 16);
                                textLine.setFillColor(sf::Color::White);
                                textLine.setPosition(60, yOffset);
                                displayedLines.push_back(textLine);
                                yOffset += 25;
                            }
                        }
                        else if (menuButtons[2].isMouseOver(window)) {
                            state = AppState::EditSubject_Find;
                            statusMessage.setString("");
                            findInput.clear();
                        }
                        else if (menuButtons[3].isMouseOver(window)) {
                            state = AppState::RemoveSubject;
                            statusMessage.setString("");
                            findInput.clear();
                        }
                        else if (menuButtons[4].isMouseOver(window)) {
                            state = AppState::GeneratePlan;
                            vector<string> data = generateStudyPlanForDisplay(list);
                            displayedLines.clear();
                            float yOffset = 110;
                            for(const string& line : data) {
                                sf::Text textLine(line, font, 16);
                                textLine.setFillColor(sf::Color::White);
                                textLine.setPosition(60, yOffset);
                                displayedLines.push_back(textLine);
                                yOffset += 25;
                            }
                        }
                        else if (menuButtons[5].isMouseOver(window)) {
                            StackNode* act = undoStack.pop();
                            if (!act) {
                                statusMessage.setString("Nothing to undo.");
                            } else {
                                if (act->action == "add") {
                                    list.removeSubject(act->subject->name);
                                    statusMessage.setString("Undo Add: " + act->subject->name);
                                } else if (act->action == "remove") {
                                    list.addSubject(act->subject->name, act->subject->difficulty,
                                        act->subject->days_left, act->subject->hours);
                                    statusMessage.setString("Undo Remove: " + act->subject->name);
                                }
                                delete act->subject;
                                delete act;
                            }
                        }
                        else if (menuButtons[6].isMouseOver(window)) {
                            window.close();
                        }
                    }
                    else if (state == AppState::AddSubject) {
                        if (backButton.isMouseOver(window)) {
                            state = AppState::MainMenu;
                        }
                        else if (submitButton.isMouseOver(window)) {
                            try {
                                string name = addInputs[0].inputString;
                                int diff = stoi(addInputs[1].inputString);
                                int days = stoi(addInputs[2].inputString);
                                int hours = stoi(addInputs[3].inputString);
                                if (name.empty()) {
                                    statusMessage.setString("Error: Name cannot be empty.");
                                } else if (list.findSubject(name)) {
                                    statusMessage.setString("Error: Subject already exists!");
                                } else if (diff < 1 || diff > 10) {
                                    statusMessage.setString("Error: Difficulty must be 1-10.");
                                } else if (days < 1 || days > 30) {
                                    statusMessage.setString("Error: Days must be 1-30.");
                                } else if (hours < 1 || hours > 24) {
                                    statusMessage.setString("Error: Hours must be 1-24.");
                                } else {
                                    list.addSubject(name, diff, days, hours);
                                    undoStack.push("add", new Subject(name, diff, days, hours));
                                    statusMessage.setString("Subject added: " + name);
                                    state = AppState::MainMenu;
                                }
                            } catch (const exception& e) {
                                statusMessage.setString("Error: Invalid number input or missing value.");
                            }
                        }
                    }
                    else if (state == AppState::RemoveSubject) {
                        if (backButton.isMouseOver(window)) {
                            state = AppState::MainMenu;
                        }
                        else if (removeButton.isMouseOver(window)) {
                            string name = findInput.inputString;
                            Subject* s = list.findSubject(name);
                            if (s) {
                                undoStack.push("remove", new Subject(s->name, s->difficulty, s->days_left, s->hours));
                                list.removeSubject(name);
                                statusMessage.setString("Subject removed: " + name);
                                state = AppState::MainMenu;
                            } else {
                                statusMessage.setString("Error: Subject not found.");
                            }
                        }
                    }
                    else if (state == AppState::EditSubject_Find) {
                        if (backButton.isMouseOver(window)) {
                            state = AppState::MainMenu;
                        }
                        else if (findButton.isMouseOver(window)) {
                            string name = findInput.inputString;
                            Subject* s = list.findSubject(name);
                            if (s) {
                                subjectToEdit = s->name;
                                addInputs[0].inputString = s->name;
                                addInputs[0].text.setString(s->name);
                                addInputs[1].inputString = to_string(s->difficulty);
                                addInputs[1].text.setString(to_string(s->difficulty));
                                addInputs[2].inputString = to_string(s->days_left);
                                addInputs[2].text.setString(to_string(s->days_left));
                                addInputs[3].inputString = to_string(s->hours);
                                addInputs[3].text.setString(to_string(s->hours));
                                state = AppState::EditSubject_Edit;
                            } else {
                                statusMessage.setString("Error: Subject not found.");
                            }
                        }
                    }
                    else if (state == AppState::EditSubject_Edit) {
                        if (backButton.isMouseOver(window)) {
                            state = AppState::MainMenu;
                        }
                        else if (updateButton.isMouseOver(window)) {
                            try {
                                int diff = stoi(addInputs[1].inputString);
                                int days = stoi(addInputs[2].inputString);
                                int hours = stoi(addInputs[3].inputString);
                                if (diff < 1 || diff > 10) {
                                    statusMessage.setString("Error: Difficulty must be 1-10.");
                                } else if (days < 1 || days > 30) {
                                    statusMessage.setString("Error: Days must be 1-30.");
                                } else if (hours < 1 || hours > 24) {
                                    statusMessage.setString("Error: Hours must be 1-24.");
                                } else {
                                    Subject* s = list.findSubject(subjectToEdit);
                                    if(s) {
                                        s->difficulty = diff;
                                        s->days_left = days;
                                        s->hours = hours;
                                        s->updatePriority();
                                        statusMessage.setString("Subject updated: " + s->name);
                                        state = AppState::MainMenu;
                                    }
                                }
                            } catch (const exception& e) {
                                statusMessage.setString("Error: Invalid number input or missing value.");
                            }
                        }
                    }
                    else if (state == AppState::ViewSubjects || state == AppState::GeneratePlan) {
                        if (backButton.isMouseOver(window)) {
                            state = AppState::MainMenu;
                        }
                    }
                }
            }
        }
        if (state == AppState::MainMenu) {
            for (auto& btn : menuButtons) btn.update(window);
        }
        else if (state == AppState::AddSubject) {
            submitButton.update(window);
            backButton.update(window);
        }
        else if (state == AppState::RemoveSubject) {
            removeButton.update(window);
            backButton.update(window);
        }
        else if (state == AppState::EditSubject_Find) {
            findButton.update(window);
            backButton.update(window);
        }
        else if (state == AppState::EditSubject_Edit) {
            updateButton.update(window);
            backButton.update(window);
        }
        else if (state == AppState::ViewSubjects || state == AppState::GeneratePlan) {
            backButton.update(window);
        }
        window.clear();
        window.draw(backgroundSprite);
        window.draw(title);
        window.draw(statusMessage);
        switch (state) {
            case AppState::MainMenu:
                for (auto& btn : menuButtons) btn.draw(window);
                break;
            case AppState::AddSubject:
                for (auto& input : addInputs) input.draw(window);
                submitButton.draw(window);
                backButton.draw(window);
                break;
            case AppState::ViewSubjects:
            case AppState::GeneratePlan:
                window.draw(displayBackground);
                for(const auto& line : displayedLines) {
                    window.draw(line);
                }
                backButton.draw(window);
                break;
            case AppState::RemoveSubject:
                findInput.draw(window);
                removeButton.draw(window);
                backButton.draw(window);
                break;
            case AppState::EditSubject_Find:
                findInput.draw(window);
                findButton.draw(window);
                backButton.draw(window);
                break;
            case AppState::EditSubject_Edit:
                for (auto& input : addInputs) input.draw(window);
                updateButton.draw(window);
                backButton.draw(window);
                break;
        }
        window.display();
    }
    return 0;
}
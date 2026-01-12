# 📝 Smart Study Planner

**Third Semester DSA Project**

Smart Study Planner is a desktop application built using **C++ and SFML** that helps students manage their study schedules efficiently. This project applies core **data structures** to solve a real-world productivity problem, focusing on performance, usability, and scalability. 💡

---

## 🚀 Features

- ➕ **Add, Edit, and Remove Subjects** with dynamic priority calculation  
- ↩️ **Undo functionality** for last action using a Stack  
- 📊 **Priority-based task management** using a custom Priority Queue  
- 🎨 **State-driven GUI** for smooth navigation  
- 🗓 **Smart Study Plan Generation** based on urgency and available study hours  

---

## 🏗 System Architecture & Data Structures

| Component            | Data Structure        | Time Complexity |
|----------------------|--------------------|----------------|
| Dynamic Subject List | Linked List          | Insertion/Deletion: O(n) |
| Undo Feature         | Stack               | Push/Pop: O(1) |
| Priority Ranking     | Priority Queue      | Insertion: O(n), Retrieve Max: O(1) |
| GUI Navigation       | State-Driven System | N/A            |

**Engineering Insight:** Optimized data structures were selected to minimize computational overhead and enhance performance. 🔧

---

## 💻 Technologies & Tools

- **C++** – Object-Oriented Programming & memory management  
- **SFML** – GUI rendering and event-driven programming  
- **Custom Makefile** – Automates compilation & static linking  
- **Static Build** – All dependencies embedded in the executable for portability 🖥  

---

## 🖼 Screenshots

**Main Menu – Smart Study Planner GUI**  
![Smart Study Planner](./b52524af-bf37-4253-bad1-4797f499aad0.png)

- Clean, intuitive main menu  
- Buttons for **Add**, **View**, **Edit**, **Remove**, **Generate Study Plan**, **Undo**, and **Exit**  
- Immersive background and UI using **SFML**  

---

## 🛠 MAKEFILE
<code>all: compile link
compile:
	g++ -c main.cpp -IC:\SFML-2.4.2\include -DSFML_STATIC
link:
	g++ main.o -o Smart_Study -LC:\SFML-2.4.2\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype -ljpeg -lopengl32 -lgdi32 -lwinmm -mwindows -lsfml-main
clean:
	del /f /q Smart_Study.exe *.o</code>

- Compile & link:<br>
<code>make all</code>
- Clean build artifacts:<br>
<code>make clean</code>

⚙ How It Works
---
1. Main Menu – Navigate between all functionalities

2. Add Subject – Input name, difficulty, days left, hours, then submit

3. View Subjects – See subjects sorted by priority (difficulty × urgency)

4. Edit Subject – Search for a subject, update its attributes dynamically

5. Remove Subject – Search and remove a subject, with Undo option

6. Generate Study Plan – Outputs priority-based study plan with ranked order

7. Undo Last Action – Reverts the most recent add/remove action

- State Management – All UI screens are state-driven for smooth transitions

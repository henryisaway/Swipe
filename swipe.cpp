#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include "colours.h"

using string = std::string;
namespace filesystem = std::filesystem;

class Task{
private:
	string m_name;
	string m_description;
	int m_status;

public:
	Task(string& name, string& description, string& status){
		m_name = name;
		m_description = description;
		int temp = (int) status[0];
		m_status = temp - 48;
	}

	string getName(){
		return m_name;
	}

	string getDescription(){
		return m_description;
	}

	string getStatus(){
		if(m_status == 2){
			return "Done!";
		} else if(m_status == 1){
			return "Ongoing";
		} else {
			return "Pending";
		}
	}

	int getStatusID(){
		return m_status;
	}

	void markAsDone(){
		if(m_status == 2){
			m_status = 0;
		} else {
			m_status = 2;
		}
	}

	void markAsOngoing(){
		if(m_status == 1){
			m_status = 0;
		} else {
			m_status = 1;
		}
	}

	void printTask(){
		if(m_status == 2){
			std::cout << ANSI_COLOUR_GREEN + m_name + ANSI_COLOUR_RESET << "\n";
		} else if(m_status == 1){
			std::cout << ANSI_COLOUR_YELLOW + m_name + ANSI_COLOUR_RESET << "\n";
		} else {
			std::cout << ANSI_COLOUR_RED + m_name + ANSI_COLOUR_RESET << "\n";
		}
		std::cout << m_description << "\n";
		std::cout << "This task is " << getStatus() << "\n";
	}
};

class Swipe{
private:
	// This function loads all projects into a projects map
	std::map<int, string> getProjects(){
		string path = ".swipe";
		std::map<int, string> projects;
		int i = 1;
		for(const auto& project : filesystem::directory_iterator(path)){
			projects.emplace(i, project.path().string());
			i++;
		}
		return projects;
	}

	void printProject(const string& project){
		for(int i = 7; i < project.length() - 6; i++){ // This strips the project name out of the filename
			std::cout << project[i];
		}
		std::cout << "\n";
	}

	void viewProjects(){
		int option;
		std::cout << ANSI_COLOUR_GREEN "Your projects: \n" ANSI_COLOUR_RESET;

		for(const auto& project : m_projects){
			std::cout << ANSI_COLOUR_BLUE << project.first << ANSI_COLOUR_RESET<< ". ";
			printProject(project.second);
		}
	}

	void createProject(const string& name){
		string filepath = ".swipe/" + name + ".swipe";
		std::ofstream file(filepath);
		file.close();
	}

	void addTask(string& name, string& description){
		string status = "0";
		Task task(name, description, status);

		m_tasks.emplace(m_tasks.size() + 1, task);
	}


	void saveTasks(const string& filepath){
		std::ofstream file(filepath);

		for(auto& task : m_tasks){
			file << "Task: " << task.second.getName() << "\n";
			file << "Desc: " << task.second.getDescription() << "\n";
			file << "Stat: " << task.second.getStatusID() << "\n";	
		}
		
		file.close();
	}

	void loadTasks(const string& filepath){
		m_tasks.clear();

		std::ifstream file(filepath);
		if(!file.is_open()){
			std::cout << "There was an error loading the project file.\n";
		}

		string line, name, desc, stat;
		int i = 1;
		while(std::getline(file, line)){
			name = line.substr(6);
			std::getline(file, line);
			desc = line.substr(6);
			std::getline(file, line);
			stat = line.substr(6);

			m_tasks.emplace(i, Task(name, desc, stat));
			i++;
		}
	}

	void viewTasks(){
		for(auto& task : m_tasks){
			std::cout << task.first << ". ";
			task.second.printTask();
			std::cout << "-----------------\n";
		}
	}
	
	void mainMenu0(int& state){
		char action;
		bool running = 1;
		
		while(running){
			system("clear");
			
			std::cout << "Swipe v0.2 by henryisaway\nPlease report any bugs at https://github.com/henryisaway/Swipe/issues\n";

			std::cout << "+---------------------------+\n";
			std::cout << "[o] Open a project\n";
			std::cout << "[c] Create a new project\n";
			std::cout << "[d] Delete project\n";
			std::cout << "[q] Quit Swipe\n";
			std::cout << "+---------------------------+\n";
			std::cout << "What would you like to do? ";

			std::cin >> action;
			if(action == 'o'){
				viewProjects();
				std::cout << "Which project do you want to open? ";
				std::cin >> m_cursor;
				state = 1;
				running = 0;
			} else if(action == 'c'){
				// something goes here
			} else if(action == 'd'){
				// something goes here
			} else if(action == 'q'){
				state = -1;
				running = 0;
			} else {
				std::cout << "Invalid command, try again.\n";
			}
		}
	}

	void taskManagement1(int& state){
		int running = 1;
		auto project = m_projects.find(m_cursor);
		directory = project->second;

		while(running){
			system("clear");

			loadTasks(directory);
			viewTasks();

			char action;

			std::cout << "[c] Create new task\n";
			std::cout << "[o] Mark/Unmark task as 'Ongoing'\n";
			std::cout << "[d] Mark/Unmark task as 'Done'\n";
			std::cout << "[r] Remove task\n";
			std::cout << "[b] Back to main menu\n";
			std::cout << "[q] Quit Swipe\n";
			std::cout << "What would you like to do? ";

			std::cin >> action;

			if(action == 'c'){
				string taskName;
				std::cout << "What is the name of the task? ";
				std::getline(std::cin >> std::ws, taskName);

				string taskDesc;
				std::cout << "What is the task description? ";
				std::getline(std::cin, taskDesc);

				addTask(taskName, taskDesc);
				saveTasks(directory);
			} else if(action == 'o'){
				std::cout << "Which task do you want to mark/unmark as ongoing? ";
				std::cin >> m_cursor;
				auto task = m_tasks.find(m_cursor);
				task->second.markAsOngoing();
				saveTasks(directory);
			} else if(action == 'd'){
				std::cout << "Which task do you want to mark/unmark as done? ";
				std::cin >> m_cursor;
				auto task = m_tasks.find(m_cursor);
				task->second.markAsDone();
				saveTasks(directory);
			} else if(action == 'r'){
				std::cout << "Which task do you want to remove? ";
				std::cin >> m_cursor;
				auto task = m_tasks.find(m_cursor);

				std::cout << "Are you sure you want to remove " << task->second.getName() << "? This action cannot be undone [y/n] ";
				char confirmation;
				std::cin >> confirmation;

				if(confirmation == 'y'){
					m_tasks.erase(task->first);
					saveTasks(directory);
					std::cout << "Task deleted, press Enter to continue.";
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cin.get();
				} else if(confirmation == 'n'){
					std::cout << "Aborted, press Enter to continue.";
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cin.get();
				} else {
					std::cout << "Unrecognized option, action was aborted. Press Enter to continue.";
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cin.get();
				}
			} else if(action == 'b'){
				state = 0;
				running = 0;
			} else if(action == 'q'){
				state = -1;
				running = 0;
			} else {
				std::cout << "Invalid command, try again.\n";
			}
		}
	}

	std::map<int, string> m_projects;
	std::map<int, Task> m_tasks;		// Tasks from the loaded project file
	string directory;
	int m_cursor;

public:
	Swipe(){
		system("clear");
		if(!filesystem::exists(".swipe")){
			filesystem::create_directory(".swipe");
		}

		m_projects = getProjects();
		m_cursor = 0;
	}

	void run(){
		int state = 0;
		int running = 1;

		while(running){
			switch(state){
			case 0:
				mainMenu0(state);
				break;
			case 1:
				taskManagement1(state);
				break;
			default:
				std::cout << "Quitting Swipe.\n";
				exit(1);
			}
		}
	}

};

int main(int argc, char** argv){
	Swipe swipe;

	if(argc < 2){
		swipe.run();
	} else {
		string arg = argv[1];
	}
	return 0;
}
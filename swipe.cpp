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
	bool m_status;

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
		if(m_status){
			return "Done!";
		} else {
			return "Pending";
		}
	}

	void printTask(){
		if(m_status){
			std::cout << ANSI_COLOUR_GREEN + m_name + ANSI_COLOUR_RESET << "\n";
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

	void createTask(string& name, string& description){
		string status = "0";
		Task task(name, description, status);
	}


	void saveTasks(const string& filepath){ // TODO make Swipe remember working directory
		std::ofstream file(filepath);

		for(auto& task : m_tasks){
			file << "Task: " << task.second.getName() << "\n";
			file << "Desc: " << task.second.getDescription() << "\n";
			file << "Stat: " << task.second.getStatus() << "\n";	
		}
		
		file.close();
	}

	void loadTasks(const string& filepath){
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

	void handleTaskManagement(){
		char action;

		std::cout << "[c] Create new task\n";
		std::cout << "[d] Delete task\n";
		std::cout << "[m] Mark task as done\n";
		std::cout << "[q] Quit Swipe\n";
		std::cout << "What would you like to do? ";

		while(1){
			std::cin >> action;

			if(action == 'q'){
				std::cout << "Quitting Swipe.\n";
				exit(0);
			} else {
				std::cout << "Invalid command, try again.\n";
			}
		}
	}

	std::map<int, string> m_projects;
	std::map<int, Task> m_tasks;		// Tasks from the loaded project file
	string directory;

public:
	Swipe(){
		system("clear");
		if(!filesystem::exists(".swipe")){
			filesystem::create_directory(".swipe");
		}

		m_projects = getProjects();
	}

	void run(){
		char action;
		int index;
		bool running = 1;
		std::cout << "Swipe v0.1 by henryisaway\nPlease report any bugs at https://github.com/henryisaway/Swipe/issues\n\n";

		std::cout << "+---------------------------+\n";
		std::cout << "[o] Open a project\n";
		std::cout << "[c] Create a new project\n";
		std::cout << "[d] Delete project\n";
		std::cout << "[q] Quit Swipe\n";
		std::cout << "+---------------------------+\n";
		std::cout << "What would you like to do? ";

		while(running){
			std::cin >> action;
			if(action == 'o'){
				viewProjects();
				std::cout << "Which project do you want to open? ";
				std::cin >> index;

				system("clear");
				auto item = m_projects.find(index);
				directory = item->second;

				loadTasks(directory);
				viewTasks();

				handleTaskManagement();
			} else if(action == 's'){
				//something goes here
			} else if(action == 'd'){
				//something goes here
			} else if(action == 'q'){
				std::cout << "Quitting Swipe.\n";
				running = 0;
			} else {
				std::cout << "Invalid command, try again.\n";
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
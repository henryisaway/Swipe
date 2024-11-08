#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <unordered_map>
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

	char getStatus(){
		int ascii = m_status;
		return (char)(ascii + 48);
	}

	void printTask(){
		std::cout << m_name << "\n";
		std::cout << m_description << "\n";
		std::cout << m_status << "\n";
	}
};

class Swipe{
private:
	std::unordered_map<int, string> getProjects(){
		string path = ".swipe";
		std::unordered_map<int, string> projects;
		int i = 1;
		for(const auto& project : filesystem::directory_iterator(path)){
			std::pair<int, string> newProject(i, project.path().string());
			projects.insert(newProject);
			i++;
		}
		return projects;
	}

	void printProject(const string& project){
		for(int i = 7; i < project.length() - 6; i++){
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
		int i = 0;
		while(std::getline(file, line)){
			name = line.substr(6);
			std::getline(file, line);
			desc = line.substr(6);
			std::getline(file, line);
			stat = line.substr(6);

			Task task(name, desc, stat);
			std::pair<int, Task> taskpair(i, task);
			m_tasks.insert(taskpair);
			i++;
		}
	}

	std::unordered_map<int, string> m_projects;
	std::unordered_map<int, Task> m_tasks;		// Tasks from the loaded project file
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
		char option;
		bool running = 1;
		std::cout << "Swipe v0.1 by henryisaway\nPlease report any bugs at https://github.com/henryisaway/Swipe/issues\n\n";
		viewProjects();

		std::cout << "\n+---------------------------+\n";
		std::cout << "[o] Open a project\n";
		std::cout << "[s] Start new project\n";
		std::cout << "[d] Delete project\n";
		std::cout << "[q] Quite Swipe\n";
		std::cout << "What would you like to do? ";

		while(running){
			std::cin >> option;
			if(option == 'o'){
				//something goes here
			} else if(option == 's'){
				//something goes here
			} else if(option == 'd'){
				//something goes here
			} else if(option == 'q'){
				std::cout << "Quitting Swipe.\n";
				running = 0;
			}
			 else {
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
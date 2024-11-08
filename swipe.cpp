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

	void createTask(const string& name, const string& description){
		//Task task(name, description);
	}


	std::unordered_map<int, string> m_projects;
	std::unordered_map<int, Task> m_tasks;
	unsigned int taskIndex;

public:
	Swipe(){
		system("clear");
		if(!filesystem::exists(".swipe")){
			filesystem::create_directory(".swipe");
		}

		m_projects = getProjects();
	}

	void saveTask(const string& filepath, Task& task){
		std::ofstream file(filepath);

		file << "Task: " << task.getName() << "\n";
		file << "Desc: " << task.getDescription() << "\n";
		file << "Stat: " << task.getStatus() << "\n";

		file.close();
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
	
	string name = "Test";
	string desc = "This task is a test.";
	string stat = "0";
	Task task(name, desc, stat);

	if(argc < 2){
		swipe.saveTask(".swipe/Jynx.swipe", task);
	} else {
		string arg = argv[1];
	}
	return 0;
}
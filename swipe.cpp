#include <iostream>
#include <cstdlib>
#include <vector>
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
	bool m_done;

public:
	Task(string name, string description){
		m_name = name;
		m_description = description;
		m_done = 0;
	}

	string getName(){
		return m_name;
	}

	string getDescription(){
		return m_description;
	}

	int getStatus(){
		return m_done;
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

	std::unordered_map<int, string> m_projects;

public:
	Swipe(){
		//system("clear");
		if(!filesystem::exists(".swipe")){
			filesystem::create_directory(".swipe");
		}
		m_projects = getProjects();
	}

	void printProject(const string& project){
		for(int i = 7; i < project.length() - 6; i++){
			std::cout << project[i];
		}
		std::cout << "\n";
	}

	void mainMenu(){
		int option;
		std::cout << ANSI_COLOUR_GREEN "Your projects: \n" ANSI_COLOUR_RESET;

		for(const auto& project : m_projects){
			std::cout << ANSI_COLOUR_BLUE << project.first << ANSI_COLOUR_RESET<< ". ";
			printProject(project.second);
		}

		std::cout << "Which project do you want to swipe through today? ";
		std::cin >> option;
	}

	void createProject(const string& name){
		string filepath = ".swipe/" + name + ".swipe";
		std::ofstream file(filepath);
		file.close();
	}

	void createTask(const string& name, const string& description){
		Task task(name, description);
	}
};

int main(int argc, char** argv){
	Swipe swipe;
	if(argc < 2){
		swipe.mainMenu();
		swipe.createProject("Jynx");
	} else {
		string arg = argv[1];
	}
	return 0;
}
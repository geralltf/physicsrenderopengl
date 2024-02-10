#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>

#include "ShaderProgram.h"

// Define available file changes
enum class FileStatus { created, modified, erased };

class FileWatcher 
{
public:
	// ...
	const int delay = 5;
	const std::function<void(std::string, FileStatus)>* action;
	std::string* observe_directory;

	   // Monitor "path_to_watch" for changes and in case of a change execute the user supplied "action" function
	void start(const std::function<void(std::string, FileStatus)>& _action, std::string* _observe_directory) {
		this->action = &_action;
		this->observe_directory = _observe_directory;
	}
	void action_funct(std::string* file_name, FileStatus file_status)
	{
		//std::cout << "file system changes '" << file_name << "' ";

		std::filesystem::path filePath = *file_name;

		if (file_status == FileStatus::created) {
			//std::cout << "created" << std::endl;
		}
		if (file_status == FileStatus::modified) {
			//glfwMakeContextCurrent(window_context);
			std::cout << "file system changes '" << *file_name << "' ";
			if (filePath.extension() == ".vs")
			{
				std::cout << "SHADER modified" << std::endl;

				ShaderProgram::shader_hot_reload(file_name, SHADER_TYPE::VertexShader);
			}
			else if (filePath.extension() == ".fs")
			{
				std::cout << "SHADER modified" << std::endl;

				ShaderProgram::shader_hot_reload(file_name, SHADER_TYPE::FragmentShader);
			}
			else
			{
				std::cout << "modified" << std::endl;

			}
		}
		if (file_status == FileStatus::erased) {
			std::cout << "file system changes '" << *file_name << "' ";
			std::cout << "erased" << std::endl;
		}
	}
	void poll_watch()
	{
		// Wait for "delay" milliseconds
		//std::this_thread::sleep_for(std::chrono::seconds(delay));

		auto it = paths_.begin();
		while (it != paths_.end()) 
		{
			if (!std::filesystem::exists(it->first)) 
			{
				(*action)(it->first, FileStatus::erased);
				it = paths_.erase(it);
			}
			else 
			{
				it++;
			}
		}

		// Check if a file was created or modified
		for (auto& file : std::filesystem::recursive_directory_iterator(*observe_directory)) 
		{
			auto current_file_last_write_time = std::filesystem::last_write_time(file);

			// File creation
			if (!contains(file.path().string())) 
			{
				paths_[file.path().string()] = current_file_last_write_time;
				std::string file_name = file.path().string();
				action_funct(&file_name, FileStatus::created);
				// File modification

			}
			else 
			{
				if (paths_[file.path().string()] != current_file_last_write_time) 
				{
					paths_[file.path().string()] = current_file_last_write_time;
					std::string file_name = file.path().string();
					action_funct(&file_name, FileStatus::modified);

				}
			}
		}
	}
private:
	std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
	bool running_ = true;
	// Check if "paths_" contains a given key
	// If your compiler supports C++20 use paths_.contains(key) instead of this function
	bool contains(const std::string& key) {
		auto el = paths_.find(key);
		return el != paths_.end();
	}
};

#endif

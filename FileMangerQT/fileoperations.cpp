#include "fileoperations.h"

FileOperations::FileOperations()
{

}
void FileOperations::paste(fs::path source_path, fs::path destination_path, CopyCutAction action)
{
    if (!fs::exists(source_path)) {
        qInfo() << "Source file does not exist!\n" ;
        return ;
    }
    if(action == CopyCutAction::Copy)
    {
        try {
            destination_path = destination_path / source_path.filename();
            if (fs::exists(destination_path)) {
                qInfo() << "Destination already exists!\n";
                return ;
            }
            if (fs::is_directory(source_path))
            {
                fs::copy(source_path, destination_path, fs::copy_options::recursive);
            }
            else
            {
                fs::copy_file(source_path, destination_path);
            }
            return ;
        }
        catch (const std::exception& ex) {
            qInfo() << "Error: "  ;
            std::cout<<destination_path;
        }
    }
    else if(action == CopyCutAction::Cut)
    {
        destination_path = destination_path / m_cutPath.filename();
        pasteFromCut(destination_path);
    }
}
void FileOperations::pasteFromCut(fs::path destination_path)
{
    try {
        if (fs::exists(destination_path)) {
            qInfo() << "Destination already exists!\n";
            return;
        }

        if (m_cutPath.empty()) {
            return; // Nothing to paste
        }

        fs::rename(m_cutPath, destination_path);
        m_cutPath.clear();

    } catch (const std::exception& ex) {
        qInfo() << "Error: " << ex.what() << '\n';
    }
}
void FileOperations::del(fs::path filePath)
{
    if (!fs::exists(filePath))
    {
        qInfo() << "File does not exist!";
        return ;
    }
    try
    {
        if (fs::is_directory(filePath))
        {

            fs::remove_all(filePath);
            qInfo() << "directory deleted successfully!";
        }
        else
        {
            fs::remove(filePath);
            qInfo() << "File deleted successfully!";
        }


    }
    catch (const std::exception& ex)
    {
        qInfo() << "Error deleting";
    }
}
void FileOperations::cutFile(const fs::path &path)
{
    m_cutPath = path;
}

void FileOperations::renameFile(const boost::filesystem::path &path ,const std::string& newFileName ){

    qInfo() << "File path = " << QString::fromStdString(path.string()) <<"  " << QString::fromStdString(newFileName);
    std::string temp_path = removeNameFromPath(path.string());
    std::string new_path_str  = temp_path + newFileName ;

    qInfo() << "File path = " << QString::fromStdString(temp_path) << QString::fromStdString(new_path_str);
    fs::path new_path_p(new_path_str);
    try
    {
        fs::rename(path,new_path_p);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error renaming file: " << ex.what() << std::endl;
    }

}

void FileOperations::batchRenameFile( std::vector< std::string>& oldPaths,const std::string &newBaseName){
    unsigned int counter = 1 ;

    /*to be used by wafyola*/
    std::vector< std::string> new_paths ;
    std::string tempName ;
    for ( auto & path : oldPaths)
    {
        tempName = newBaseName ;
        FileOperations::renameFile(path,tempName.append(std::to_string(counter)));
        new_paths.push_back(tempName.append(std::to_string(counter)));
        counter++;
    }
    FileOperations::addPaths( oldPaths,new_paths);

}


std::string FileOperations:: removeNameFromPath(std::string path) {
    size_t found = path.find_last_of("/\\");
    if (found != std::string::npos) {
        return path.substr(0, found+1);
    }
    return path;
}
void FileOperations::addPaths(std::vector<std::string> oldPaths, std::vector<std::string> newPaths)
{
//    paths.push(oldPaths);
//    paths.push(newPaths);
}

#include "tools-pass-fox.hpp"

ToolsPassFox::ToolsPassFox(){
  home = getenv("HOME");
  file = {"key4.db"};
  src = get_filename( file );
  tmp = "/tmp/pass-fox-kajsdjs8";
  des = tmp + "/" + file;
  passfirefox = home + "/pass-firefox.gpg";
}

void ToolsPassFox::make_dir_files(){
  if( ! fs::exists( tmp ) ){
    fs::create_directories( tmp );
  }
}

bool ToolsPassFox::fake_zlib(){
  std::string com = "tar -zcf " + tmp + ".tar.gz " + " -P " + tmp;
  char * ccom = &com[0];
  return system( ccom );
}

bool ToolsPassFox::fake_gpgme( int action, std::string filepass ){
  if( filepass == "" ){
    return false;
  }

  std::string com;
  char * ccom;

  switch ( action ) {
    case 0:
      com = "gpg --yes --batch --passphrase=" + filepass + " -c " + tmp + ".tar.gz";
      ccom = &com[0];
      system( ccom );
      return true;
      break;
    case 1:
      com = "gpg --quiet --yes --batch --passphrase=" + filepass + " " + tmp + "/pass-firefox.gpg";
      ccom = &com[0];
      system( ccom );
      return fs::exists( tmp + "/pass-firefox" );
      break;
  }

  return false;

}

std::string ToolsPassFox::get_filename( std::string filename ){
  std::string dir = home + "/.mozilla/firefox", 
  pathfile = "find " + dir + " -name " + filename + " > /tmp/kasjdhakshd",
  line;

  char * cpathfile = &pathfile[0];
  int stream_file = system( cpathfile );
  std::ifstream file( "/tmp/kasjdhakshd" );

  file.is_open();
  while( getline( file, line ) ){
    pathfile = line;
  }
  file.close();

  if( stream_file == 0 ){
    fs::remove( "/tmp/kasjdhakshd" );
  }

  return pathfile;
}

void ToolsPassFox::save_file(){
  fs::copy_file( tmp + ".tar.gz.gpg" , passfirefox, fs::copy_options::overwrite_existing );
  fs::remove_all( tmp );
  fs::remove( tmp + ".tar.gz" );
  fs::remove( tmp + ".tar.gz.gpg" );
}

bool ToolsPassFox::export_file( std::string pass ){
  make_dir_files();
  fs::copy_file( src, des, fs::copy_options::overwrite_existing );
  file = "logins.json";
  src = get_filename( file );
  des = tmp + "/" + file;
  fs::copy_file( src, des, fs::copy_options::overwrite_existing );
  fake_zlib();
  filepass = pass; 
  fake_gpgme( 0, filepass );
  save_file();
  return fs::exists( passfirefox );
}

bool ToolsPassFox::import_file( std::string filename, std::string pass ){
  if( ! fs::exists( home + "/.mozilla/firefox" ) ){
    return false;
  }
  
  make_dir_files();
  fs::copy_file( filename, tmp + "/pass-firefox.gpg", fs::copy_options::overwrite_existing );

  if( ! fake_gpgme( 1, pass ) ){
    fs::remove_all( tmp );
    return false;
  }

  std::string com = "tar -zxf " + tmp + "/pass-firefox " + " -C " + tmp + " 2>/dev/null";
  char * ccom = &com[0];
  system( ccom );

  std::string to_path = get_filename( file );
  to_path = to_path.substr( 0, to_path.length() - 7 );
  fs::copy_file( tmp + tmp + "/key4.db", to_path + "key4.db", fs::copy_options::overwrite_existing );
  fs::copy_file( tmp + tmp + "/logins.json", to_path + "logins.json", fs::copy_options::overwrite_existing );
  fs::remove_all( tmp );

  return true;

}

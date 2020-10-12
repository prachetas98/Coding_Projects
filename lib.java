public class lib{
	String lib_name;
	int shelf;

	public lib(String lib_name,int shelf){
		this.lib_name = lib_name;
		this.shelf = shelf;
	}

	public lib(){}

	public String get_lib_name(){
		return lib_name;
	}

	public int get_shelf(){
		return shelf;
	}
}

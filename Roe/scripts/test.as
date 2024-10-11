class TestClass : ComponentScript {

	int ax = 0;
	
	void update() {
		print("Hello from Test Class id:" + UID + " : " + ax);
		ax++;
	}
}
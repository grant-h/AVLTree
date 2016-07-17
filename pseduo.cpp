// bbst.cpp
main(string args)
{
  // find in util.cpp
  list lines = ReadInput(args.file);

  // find in AVLTree.cpp (BuildFromSortedList)
  Tree t = CreateTreeFromLines(lines);

  while(stdin) {
    String cmd = Read();

    // comparison of 
    cmd match {
      // AVLTree.cpp line 153
      case "increase" => t.Increase(cmd)
        ...
      // AVLTree.cpp line 503
      case "inrange" => t.InRange(cmd)
      case "quit" => Quit()
    }
  }
}

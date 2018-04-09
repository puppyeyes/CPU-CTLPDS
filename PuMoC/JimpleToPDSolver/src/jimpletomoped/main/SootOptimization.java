package jimpletomoped.main;


public enum SootOptimization {
	CommonSubexpressionEliminator,
    ConstantPropagatorAndFolder,
    DeadAssignmentEliminator,
    ConditionalBranchFolder,
    UnconditionalBranchFolder,
    UnreachableCodeEliminator,
    JimpleConstructorFolder,
    ThisInliner,
    LocalPacker,
    UnusedLocalEliminator,   
}

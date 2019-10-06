 J=0.0;

forAll(patchesToIntegrate, i)

{
    label patchi = mesh.boundaryMesh().findPatchID(patchesToIntegrate[i]);
    J = gSum(
        (U.boundaryField()[patchi])&mesh.magSf().boundaryField()[patchi] * ((p.boundaryField()[patchi])+0.5*(U.boundaryField()[patchi])*(U.boundaryField()[patchi])));
}

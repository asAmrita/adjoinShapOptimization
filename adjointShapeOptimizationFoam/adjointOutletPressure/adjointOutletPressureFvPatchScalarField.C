/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "adjointOutletPressureFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchMapper.H"
#include "volFields.H"
#include "surfaceFields.H"

#include "turbulentTransportModel.H"
#include "RASModel.H" // AK: change 1

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::adjointOutletPressurePowerFvPatchScalarField::
adjointOutletPressurePowerFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(p, iF)
{}


Foam::adjointOutletPressurePowerFvPatchScalarField::
adjointOutletPressurePowerFvPatchScalarField
(
    const adjointOutletPressurePowerFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper)
{}


Foam::adjointOutletPressurePowerFvPatchScalarField::
adjointOutletPressurePowerFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchScalarField(p, iF, dict)
{}


Foam::adjointOutletPressurePowerFvPatchScalarField::
adjointOutletPressurePowerFvPatchScalarField
(
    const adjointOutletPressurePowerFvPatchScalarField& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(tppsf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::adjointOutletPressurePowerFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

     const fvsPatchField<scalar>& phip =
         patch().lookupPatchField<surfaceScalarField, scalar>("phi");

     const fvsPatchField<scalar>& phiap =
         patch().lookupPatchField<surfaceScalarField, scalar>("phia");


     scalarField Up_n = phip/patch().magSf();
     scalarField Uap_n = phiap/patch().magSf();

    
     const fvPatchField<vector>& Up =
         patch().lookupPatchField<volVectorField, vector>("U");

     const fvPatchField<vector>& Uap =
         patch().lookupPatchField<volVectorField, vector>("Ua");


    // scalarField Up_n = Up & patch().nf(); // primal normal
    // scalarField Uap_n = Uap & patch().nf(); // normal

    //Foam::incompressible::RASModel
//     const incompressible::RASModel& rasModel =
//        db().lookupObject<incompressible::RASModel>("turbulenceProperties");
   
     const incompressible::turbulenceModel & turb = db().lookupObject<incompressible::turbulenceModel>("turbulenceProperties");

     scalarField nueff = turb.nuEff()().boundaryField()[patch().index()];
               
   // scalarField nueff = rasModel.nuEff()().boundaryField()[patch().index()];
    const scalarField & deltainv = patch().deltaCoeffs();

    
    

    scalarField Uaneigh_n = Uap.patchInternalField() & patch().nf();
    const fvPatchField<vector> & Udp =
        patch().lookupPatchField<volVectorField, vector>("Ua");


    //scalarField Udp_n = (Udp & patch().nf());

        
    scalarField::operator= ( (Uap & Up) + (Up_n*Uap_n)
    + nueff*deltainv*(Uap_n - Uaneigh_n)
    - 0.5*mag(Up)*mag(Up)   - (Up & patch().Sf()/patch().magSf())*(Up & patch().Sf()/patch().magSf()));

    fixedValueFvPatchScalarField::updateCoeffs();
}


void Foam::adjointOutletPressurePowerFvPatchScalarField::write(Ostream& os) const
{
    fvPatchScalarField::write(os);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        adjointOutletPressurePowerFvPatchScalarField
    );
}

// ************************************************************************* //

		// find ID of patch
		label patchID = mesh.boundaryMesh().findPatchID(interfaceName);
		// check patch has been found
		if(patchID == -1)
		{
				FatalError << "patch not found!" << exit(FatalError);
		}

		//calculate the heat flux of the inerface
		volScalarField& he = thermo.he();
		const volScalarField& alpha = thermo.alpha();

		//the heat flux of each surfaces of each volumes
		heatFlux = fvc::interpolate(alpha)*fvc::snGrad(he);

		//change the surface heat flux as the volume(vol) heat flux
		volScalarField::Boundary& wallHeatFluxBf = wallHeatFlux.boundaryFieldRef();

		const surfaceScalarField::Boundary& heatFluxBf = heatFlux.boundaryField();

		//calculate the mass flux of the interface

		scalar averageHeatBC = 0;
		scalar averageMassBC = 0;

		scalar minHfp = 0;
		scalar maxHfp = 0;
		scalar meanHfp = 0;

		//find the cylinder patch to calculate the heat flux
		wallHeatFluxBf[patchID] = heatFluxBf[patchID];
		wallHeatFlux = -wallHeatFlux;
		const scalarField& hfp = wallHeatFlux.boundaryField()[patchID];
		//the average heat
		minHfp = gMin(hfp);
		maxHfp = gMax(hfp);
		meanHfp = (minHfp+maxHfp)/2;
		averageHeatBC = meanHfp;

		Info<<endl;
		Info<<"The minimun value of "<<patchID<< " the heat flux is "<<minHfp<<endl;
		Info<<"The maximun value of "<<patchID<<" the heat flux is "<<maxHfp<<endl;
		Info<<endl;


		Info<<"The average value of the heat flux is "<<averageHeatBC<<endl;
		Info<<endl;

		//update the fuel
		//Define the fuel boundary condition object
		fvPatchScalarField& fuel_patch = refCast<fvPatchScalarField>(composition.Y("C7H16").boundaryFieldRef()[patchID]);

		//Cast calculated fuel to each cell on the patch
		forAll(mesh.boundaryMesh()[patchID].faceCentres(), faceI)
		{
			//Get the coordinate
			const scalar x = mesh.boundaryMesh()[patchID].faceCentres()[faceI].x();
			const scalar y = mesh.boundaryMesh()[patchID].faceCentres()[faceI].y();
			if (y<0.2 && y>0.1){
				fuel_patch[faceI] = 0.05;
			}
			else{
				fuel_patch[faceI] = 0;
			}
		}
		Info<<"The fuel changes"<<endl;


		Info<<"B number is "<<analyticalRegion::B<<endl;




		Info<<"The index is "<<	assimilationTimeIndex <<endl;
		Info<<"The time is "<<	data[assimilationTimeIndex][0] <<endl;
		Info<<"The observation is "<<data[assimilationTimeIndex][1] <<endl;

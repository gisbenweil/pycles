#pragma once
#include "grid.h"

void compute_velocity_gradient(const struct DimStruct *dims, double* restrict v, double* restrict vgrad, const long d){

    const long istride = dims->nlg[1] * dims->nlg[2];
    const long jstride = dims->nlg[2];

    const long imin = 0;
    const long jmin = 0;
    const long kmin = 0;

    const long imax = dims->nlg[0]-1;
    const long jmax = dims->nlg[1]-1;
    const long kmax = dims->nlg[2]-1;

    const long stencil[3] = {istride,jstride,1};
    const long sp1 = stencil[d];
    const double dxi = dims->dxi[d];


    for(long i=imin;i<imax;i++){
        const long ishift = i*istride ;
        for(long j=jmin;j<jmax;j++){
            const long jshift = j*jstride;
            for(long k=kmin;k<kmax;k++){
                const long ijk = ishift + jshift + k ;
                vgrad[ijk] = (v[ijk + sp1] - v[ijk])*dxi;
            }
        }
    }
    return;
}


void compute_strain_rate(const struct DimStruct *dims, double* restrict vgrad, double* restrict strain_rate){

    const long istride = dims->nlg[1] * dims->nlg[2];
    const long jstride = dims->nlg[2];

    const long imin = 0;
    const long jmin = 0;
    const long kmin = 0;

    const long imax = dims->nlg[0]-1;
    const long jmax = dims->nlg[1]-1;
    const long kmax = dims->nlg[2]-1;
    long count = 0;
    //Loop over the dimensions twice to compute the strain rate vector components
    for(long vi1=0;vi1<dims->dims;vi1++){
        for(long d=0;d<dims->dims;d++){
            const long shift_v1 = 3 * dims->npg * vi1 + dims->npg * d ;
            const long shift_v2 = 3 * dims->npg * d + dims-> npg * vi1;
            const long shift = count * dims->npg;
            for(long i=imin;i<imax;i++){
                const long ishift = i*istride ;
                for(long j=jmin;j<jmax;j++){
                    const long jshift = j*jstride;
                    for(long k=kmin;k<kmax;k++){
                        const long ijk = ishift + jshift + k ;
                        strain_rate[shift + ijk] = 0.5 * (vgrad[shift_v1 + ijk] + vgrad[shift_v2 + ijk]) ;
                        }
                    }
                }
        count = count + 1 ;
        }
    }
    return;
}

void compute_strain_rate_mag(const struct DimStruct *dims, double* restrict strain_rate, double* restrict strain_rate_mag){

    const long istride = dims->nlg[1] * dims->nlg[2];
    const long jstride = dims->nlg[2];

    const long imin = 0;
    const long jmin = 0;
    const long kmin = 0;

    const long imax = dims->nlg[0]-1;
    const long jmax = dims->nlg[1]-1;
    const long kmax = dims->nlg[2]-1;
    long count = 0;

    //Set all values of strain rate magnitude to zero
    for(long i=0; i<dims->npg; i++){
        strain_rate_mag[i] = 0.0;
    }

    //Loop over the dimensions twice to compute the strain rate vector components
    for(long vi1=0;vi1<dims->dims;vi1++){
        for(long d=0;d<dims->dims;d++){
            const long shift_v1 = 3 * dims->npg * vi1 + dims->npg * d ;
            const long shift_v2 = 3 * dims->npg * d + dims-> npg * vi1;
            for(long i=imin;i<imax;i++){
                const long ishift = i*istride ;
                for(long j=jmin;j<jmax;j++){
                    const long jshift = j*jstride;
                    for(long k=kmin;k<kmax;k++){
                        const long ijk = ishift + jshift + k ;
                        strain_rate_mag[ijk] = strain_rate_mag[ijk] + sqrt(2.0 * strain_rate[shift_v1 + ijk] * strain_rate[shift_v2 + ijk]) ;
                        }
                    }
                }
        }
    }


    return;
}
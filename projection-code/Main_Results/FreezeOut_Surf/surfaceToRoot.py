#!/usr/bin/env python
#Author: Willian M. Serenone
#Date: September 1st, 2020
#
# Reads the freeze-out hypersurface and facilitate its access

# Hypersurface format is an array of C-floats (32 bits each entry). We should
# group them in sets of 58 floats (34 if the file does not contain vorticity
# data). Each set correspond to a fluid cell that reached the particlization
# surface and contains, in this order:
# tau_center, x_center, y_center, eta_center, s^tau, s^x, s^y, s^eta,                       ##Index 0  to 7
# u^tau, u^x, u^y, u^eta, eps, T, muB, muS, muC, (eps+P)/T,                                 ##Index 8  to 17
# Wtautau, Wtaux, Wtauy, # Wtaueta, Wxx, Wxy, Wxeta, Wyy, Wyeta, Wetaeta,                   ##Index 18 to 27
# pi_b, rho_b, Wmunu[10], Wmunu[11], Wmunu[12], Wmunu[13],                                  ##Index 28 to 33
# omegaSP_tx, omegaSP_ty, omegaSP_teta, omegaSP_xy, omegaSP_xeta, omegaSP_yeta,             ##Index 34 to 39
# omega_k_tx, omega_k_ty, omega_k_teta, omega_k_xy, omega_k_xeta, omega_k_yeta,             ##Index 40 to 45
# omega_th_tx, omega_th_ty, omega_th_teta, omega_th_xy, omega_th_xeta, omega_th_yeta,       ##Index 46 to 51
# omega_T_tx, omega_T_ty, omega_T_teta, omega_T_xy, omega_T_xeta, omega_T_yeta              ##Index 52 to 57

import os
import psutil
import sys
import numpy as np
import ROOT as R

def load_args():
    nargs = 2
    input = list()
    for ii in range(1,len(sys.argv)):
        input.append(sys.argv[ii])

    if len(input) != nargs:
        print("Wrong number of parameters passed. Syntax hould be:")
        print(sys.argv[0]+" surface.dat output_path.root")
        sys.exit(1)
    
    return input

class Hypersurface():
    def __init__(self, freeze_path):
        self.__data_per_cell = 58

        self.freeze_path = freeze_path

        self.__freeze_in_RAM = self.__decide_load_file_RAM()
        
        self.__freezeout_surface_array = 0
        self.__load_freezeout()
        
        nfloats = len(self.__freezeout_surface_array)
        self.ncells = nfloats//self.__data_per_cell
        self.freezeout_surface = self.__freezeout_surface_array.reshape((self.ncells,self.__data_per_cell),order='C')

        #Buffers to speed-up methods computations
        self.__cosheta = np.cosh(self.get_eta_center())
        self.__sinheta = np.sinh(self.get_eta_center())
        self.__taucosheta = self.__cosheta*self.get_tau_center()
        self.__tausinheta = self.__sinheta*self.get_tau_center()

    #Private methods
    def __decide_load_file_RAM(self):
        file_size = os.path.getsize(self.freeze_path)
        RAM_available = psutil.virtual_memory()[1]
        if (file_size < RAM_available):
            return True
        else:
            return False

    def __load_freezeout(self):
        if self.__freeze_in_RAM:
            self.__freezeout_surface_array = np.fromfile(self.freeze_path,dtype=np.float32)
        else:
            self.__freezeout_surface_array = np.memmap(self.freeze_path,dtype=np.float32,mode='r')
    
    def __contravariant_milne_to_minks_t(self,vtau,veta,tau,eta):
        return vtau*self.__cosheta + veta*self.__tausinheta
    
    def __contravariant_milne_to_minks_z(self,vtau,veta,tau,eta):
        return vtau*self.__sinheta + veta*self.__taucosheta

    #Position Getters
    def get_tau_center(self):
        return self.freezeout_surface[:,0]
    def get_x_center(self):
        return self.freezeout_surface[:,1]
    def get_y_center(self):
        return self.freezeout_surface[:,2]
    def get_eta_center(self):
        return self.freezeout_surface[:,3]
    def get_t_center(self):
        return np.sqrt(self.get_tau_center()**2 + self.get_eta_cener()**2)
    def get_z_center(self):
        return np.tanh(self.get_eta_center())*self.get_t_center()

    #Fluid Velocity Getters
    def get_utau(self):
        return self.freezeout_surface[:,8]   
    def get_ux(self):
        return self.freezeout_surface[:,9]
    def get_uy(self):
        return self.freezeout_surface[:,10]
    def get_ueta(self):
        return self.freezeout_surface[:,11]
    def get_ut(self):
        return self.__contravariant_milne_to_minks_t(self.get_utau(),self.get_ueta(),
                                                    self.get_tau_center(),self.get_eta_center())
    def get_uz(self):
        return self.__contravariant_milne_to_minks_z(self.get_utau(),self.get_ueta(),
                                                    self.get_tau_center(),self.get_eta_center())
    
    #Kinectic Velocity Getters
    def get_omega_k_tx(self):
        return self.freezeout_surface[:,40]    
    def get_omega_k_ty(self):
        return self.freezeout_surface[:,41]    
    def get_omega_k_tz(self):
        return self.freezeout_surface[:,42]
    def get_omega_k_xy(self):
        return self.freezeout_surface[:,43]
    def get_omega_k_xz(self):
        return self.freezeout_surface[:,44]
    def get_omega_k_yz(self):
        return self.freezeout_surface[:,45]
        
    #Thermal Velocity Getters
    def get_omega_th_tx(self):
        return self.freezeout_surface[:,46]    
    def get_omega_th_ty(self):
        return self.freezeout_surface[:,47]    
    def get_omega_th_tz(self):
        return self.freezeout_surface[:,48]
    def get_omega_th_xy(self):
        return self.freezeout_surface[:,49]
    def get_omega_th_xz(self):
        return self.freezeout_surface[:,50]
    def get_omega_th_yz(self):
        return self.freezeout_surface[:,51]
        
def main(args):

    def spatial_rap_cut(eta_cut,eta_center):
        accept = np.zeros(len(eta_center),dtype=np.bool)
        for icell in range(len(eta_center)):
            if abs(eta_center[icell]) < eta_cut:
                accept[icell] = True
        return accept
    
    ##### HARD CODED PARAMETERS ####
    nphi = 256
    ################################

    #Get input arguments
    hypersurface_path = args[0]
    output_path = args[1]


    #Create output histograms
    hOmega_z = R.TH1D("hOmega_z","#omega_z(phi) in the particlization hypersuface", nphi, -np.pi, np.pi)
    hOmega_y = R.TH1D("hOmega_y","#omega_y(phi) in the particlization hypersuface", nphi, -np.pi, np.pi)
    hOmega_x = R.TH1D("hOmega_x","#omega_x(phi) in the particlization hypersuface", nphi, -np.pi, np.pi)
    
    hOmega_01 = R.TH1D("hOmega_01","#omega_01(phi) in the particlization hypersuface", nphi, -np.pi, np.pi)
    hOmega_02 = R.TH1D("hOmega_02","#omega_02(phi) in the particlization hypersuface", nphi, -np.pi, np.pi)
    hOmega_03 = R.TH1D("hOmega_03","#omega_03(phi) in the particlization hypersuface", nphi, -np.pi, np.pi)

    #Setup  x axis
    hOmega_z.GetXaxis().SetTitle("#it{#varphi}")
    hOmega_y.GetXaxis().SetTitle("#it{#varphi}")
    hOmega_x.GetXaxis().SetTitle("#it{#varphi}")
    hOmega_01.GetXaxis().SetTitle("#it{#varphi}")
    hOmega_02.GetXaxis().SetTitle("#it{#varphi}")
    hOmega_03.GetXaxis().SetTitle("#it{#varphi}")

    #Setup y axis
    hOmega_z.GetYaxis().SetTitle("#sum #omega^{z}")
    hOmega_y.GetYaxis().SetTitle("#sum #omega^{y}")
    hOmega_x.GetYaxis().SetTitle("#sum #omega^{x}")
    hOmega_01.GetYaxis().SetTitle("#sum #omega^{01}")
    hOmega_02.GetYaxis().SetTitle("#sum #omega^{02}")
    hOmega_03.GetYaxis().SetTitle("#sum #omega^{03}")

    #Load hypersurface (This may take a while)
    FOsurf = Hypersurface(hypersurface_path)

    #Catch the necessary arrays
    eta_center = FOsurf.get_eta_center()
    x_center = FOsurf.get_x_center()
    y_center = FOsurf.get_y_center()
    ux = FOsurf.get_ux()
    uy = FOsurf.get_uy()
    omega_z = FOsurf.get_omega_k_xy()
    omega_y = FOsurf.get_omega_k_xz()
    omega_x = FOsurf.get_omega_k_yz()
    omega_01 = FOsurf.get_omega_k_tx()
    omega_02 = FOsurf.get_omega_k_ty()
    omega_03 = FOsurf.get_omega_k_tz()

    #Define the rapidity cut 
    accept_map = spatial_rap_cut(1.0, eta_center)

    for icell in range(len(eta_center)):
        if (accept_map[icell]):
            phi = np.arctan2(uy[icell], ux[icell])
            hOmega_x.Fill(phi, omega_x[icell])
            hOmega_y.Fill(phi, omega_y[icell])
            hOmega_z.Fill(phi, omega_z[icell])

            hOmega_01.Fill(phi, omega_01[icell])
            hOmega_02.Fill(phi, omega_02[icell])
            hOmega_03.Fill(phi, omega_03[icell])

        #end if
    #end for
            
    fout = R.TFile(output_path,"RECREATE")

    hOmega_integrand = hOmega_01.Clone("hOmega_integrand");
    hOmega_integrand.Reset("ICESM")
    for ibin in range(1,hOmega_integrand.GetXaxis().GetNbins()+1):
        phi = hOmega_01.GetXaxis().GetBinCenter(ibin)
        omega01 = hOmega_01.GetBinContent(ibin)
        omega02 = hOmega_02.GetBinContent(ibin)
        s = omega01*np.sin(phi) - omega02*np.cos(phi)
        hOmega_integrand.SetBinContent(ibin,s)


    hOmega_x.SetDirectory(fout)
    hOmega_y.SetDirectory(fout)
    hOmega_z.SetDirectory(fout)

    hOmega_01.SetDirectory(fout)
    hOmega_02.SetDirectory(fout)
    hOmega_03.SetDirectory(fout)

    hOmega_integrand.SetDirectory(fout)
    
    fout.Write()
    fout.Close()

    return 0 
if __name__ == "__main__":
    args = load_args()
    main(args)
import cv2

def main():
    cap = cv2.VideoCapture(0)  # 0 untuk kamera utama (ubah jika menggunakan lebih dari satu kamera)
    
    if not cap.isOpened():
        print("Error: Tidak dapat membuka kamera.")
        return
    
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error: Gagal menangkap frame.")
            break
        
        cv2.imshow('Webcam', frame)  # Menampilkan video secara real-time
        
        if cv2.waitKey(1) & 0xFF == ord('q'):  # Tekan 'q' untuk keluar
            break
    
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()

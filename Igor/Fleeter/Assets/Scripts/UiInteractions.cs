using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.SceneManagement;

public class UiInteractions : MonoBehaviour
{
    public static bool IsPaused = false;

    public GameObject pauseMenu;
    public FirstPersonCamera firstPersonCamera;

    void Start()
    {

    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if (!IsPaused)
            {
                Pause();
            } else
            {
                Resume();
            }
        }
    }

    void Resume()
    {
        if (firstPersonCamera!= null)
        {
            firstPersonCamera.IsCameraLocked = false;
        }
        IsPaused = false;
        Cursor.visible = false;
        Cursor.lockState = CursorLockMode.Locked;
        pauseMenu.SetActive(false);
    }

    void Pause()
    {
        if (firstPersonCamera != null)
        {
            firstPersonCamera.IsCameraLocked = true;
        }
        IsPaused = true;
        Cursor.visible = true;
        Cursor.lockState = CursorLockMode.Confined;
        pauseMenu.SetActive(true);
    }

    public void OpenRealTimeVisualization()
    {
        Resume();
        SceneManager.LoadScene("Scenes/RealTimeVisualization3D");
    }

    public void Switch3D()
    {
        Resume();
        var currentScene = SceneManager.GetActiveScene().name;
        if (currentScene == "Scenes/RealTimeVisualization2D")
        {
            SceneManager.LoadScene("Scenes/RealTimeVisualization3D");
        }
    }

    public void Switch2D()
    {
        Resume();
        var currentScene = SceneManager.GetActiveScene().name;
        if (currentScene == "Scenes/RealTimeVisualization3D")
        {
            SceneManager.LoadScene("Scenes/RealTimeVisualization2D");
        }
    }

    public void OpenTracking()
    {
        Resume();
        SceneManager.LoadScene("Scenes/Tracking");
    }
}
